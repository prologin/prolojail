/*
 * Copyright (c) 2011 by Association Prologin, see AUTHORS for more details.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *     * The names of the contributors may not be used to endorse or
 *       promote products derived from this software without specific
 *       prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "jail.hh"
#include "signal.hh"

#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

jail::jail(const jail::cmd_type& cmd) : cmd_(cmd)
{
}

int jail::run()
{
  if ((child_pid_ = fork()) == -1)
    throw exec_exception("could not fork");
  else if (!child_pid_)
    exit(child_run());
  else
    return tracer_run();
}

/*
 * Prepare the syscall tracing and exec the program to be tested
 */
int jail::child_run()
{
  const char* argv[cmd_.size() + 1];
  unsigned int i = 0;

  for (; i < cmd_.size(); ++i)
    argv[i] = cmd_[i].c_str();
  argv[i] = NULL;

  ptrace(PTRACE_TRACEME, 0, NULL, NULL);
  execvp(cmd_[0].c_str(), (char* const*)argv);

  return 1;
}

/*
 * Checks the status of the child process to handle it
 */
int jail::tracer_handle_status(int status, int& signum)
{
  if (WIFEXITED(status))
    return WEXITSTATUS(status);
  else if ((WIFSIGNALED(status) || WIFSTOPPED(status)) &&
           WSTOPSIG(status) != SIGTRAP)
  {
    signum = WSTOPSIG(status);
    return (1);
  }
  else if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP)
  {
    if (!syscall_.handle())
      return (1);
    return (-1);
  }
  else
    return (-1);
}

int jail::tracer_run()
{
  pthread_t thread;
  start_watchdog(&thread);

  int signum = 0;
  int return_code;
  int status;

  timestamp_start_ = time(NULL);
  syscall_.set_pid(child_pid_);

  while (true)
  {
    if (waitpid(child_pid_, &status, 0) < 0)
    {
      throw exec_exception("waitpid error: " +
                           std::string(strerror(errno)));
    }

    if ((return_code = tracer_handle_status(status, signum)) >= 0)
      break;

    ptrace(PTRACE_SYSCALL, child_pid_, NULL, reinterpret_cast<void*>(signum));
  }

  ptrace(PTRACE_KILL, child_pid_, NULL, NULL);
  waitpid(child_pid_, &status, 0);

  int res_thread;
  pthread_join(thread, (void**) &res_thread);
  return (handle_return_code(res_thread, return_code, signum));
}

int jail::handle_return_code(size_t res_thread, int return_code, int signum)
{
  if (res_thread == 0)
  {
    if (signum == 0)
      return (return_code);
    else
    {
      std::cerr << "Terminated by a signal: "
                << signal_to_string(signum)
                << std::endl;
      return (1);
    }
  }
  else
  {
    if (res_thread & ERR_MAX_TIME)
      std::cerr << "Max time reached" << std::endl;
    if (res_thread & ERR_MAX_MEM)
      std::cerr << "Max memory reached" << std::endl;

    return (1);
  }

}

size_t jail::check_limits()
{
  return  (check_time_limit() |
           check_memory_limit());
}

size_t jail::check_time_limit()
{
  if (time_limit_ != boost::none)
  {
    size_t time = 0;

    std::string pid = boost::lexical_cast<std::string>(child_pid_);
    std::string file = "/proc/" + pid + "/stat";
    std::ifstream h(file);
    if (!h.is_open())
      return (ERR_PROCESS_TERM);

    int i;
    std::string token;
    for (i = 0; i < 14 && !h.eof(); i++)
    {
      h >> token;
    }
    h.close();

    if (i == 14)
    {
      time = boost::lexical_cast<size_t>(token) * sysconf(_SC_CLK_TCK) / 10;
    }

    if (time > time_limit_)
    {
      kill_process();
      return (ERR_MAX_TIME);
    }

    return (check_time_limit_fallback());
  }

  return (0);
}

// If the program take more than 3 times the time allocated, terminate it.
// It prevent programs from sleeping (and not using CPU time), and thus not
// being killed.
size_t jail::check_time_limit_fallback()
{
  if (time_limit_ != boost::none)
  {
    size_t timeout = 3 * (*time_limit_ / 1000);
    if ((time(NULL) - timestamp_start_) > timeout)
    {
      kill_process();
      return (ERR_MAX_TIME);
    }
  }

  return (0);
}

size_t jail::check_memory_limit()
{
  if (mem_limit_ != boost::none)
  {
    size_t mem = 0;

    std::string pid = boost::lexical_cast<std::string>(child_pid_);
    std::string file = "/proc/" + pid + "/status";
    std::ifstream h(file);
    if (!h.is_open())
      return (ERR_PROCESS_TERM);

    while (!h.eof())
    {
      std::string token;
      h >> token;

      if (token == "VmStk:" || token == "VmData:")
      {
        h >> token;
        mem += boost::lexical_cast<size_t>(token);
      }
    }
    h.close();

    if (mem > *mem_limit_)
    {
      kill_process();
      std::string size = boost::lexical_cast<std::string>(mem);
      return (ERR_MAX_MEM);
    }
  }

  return (0);
}

int jail::kill_process()
{
  return (kill(child_pid_, SIGQUIT));
}

void jail::start_watchdog(pthread_t* thread)
{
  if (pthread_create(thread, NULL, thread_entry, this) != 0)
  {
    std::cerr << "Failed to create a thread." << std::endl;
    exit(1);
  }
}

void* jail::thread_entry(void* obj)
{
  jail* pthis = (jail*) obj;
  return (pthis->thread_run());
}

void* jail::thread_run()
{
  while (true)
  {
    int res;
    if ((res = check_limits()) != 0)
    {
      if (res & ERR_PROCESS_TERM)
        pthread_exit(NULL);
      else
        pthread_exit((void*) res);
    }

    usleep(SLEEP_TIME_MS * 1000);
  }

  pthread_exit(0);
}

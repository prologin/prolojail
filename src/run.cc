#include <iostream>
#include <cstdlib>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "const.hh"
#include "option_parser.hh"
#include "print.hh"
#include "syscall_jail.hh"
#include "run.hh"

static void set_limits()
{
  struct rlimit lim;

  lim.rlim_cur = MAX_NPROC;
  lim.rlim_max = MAX_NPROC;
  if (setrlimit(RLIMIT_NPROC, &lim) == -1)
  {
    std::cerr << strerror(errno) << std::endl;
    std::cerr << "Failed to set limits." << std::endl;
    exit(EXIT_ERR);
  }
}

static pid_t run(const option_parser::cmd_type& cmd)
{
  int child_pid;

  if ((child_pid = fork()) == 0)
  {
    size_t size = cmd.size();
    const char* argv[size + 1];
    argv[size] = NULL;
    for (size_t i = 0; i < size; i++)
      argv[i] = std::string(cmd[i]).c_str();

    set_limits();
    ptrace (PTRACE_TRACEME, 0, 0, 0);

    execvp(cmd[0].c_str(), (char* const*)argv);
    std::cerr << "Failed to exec." << std::endl;
    exit(EXIT_ERR);
  }
  else if (child_pid == -1)
  {
    std::cerr << "Failed to fork." << std::endl;
    exit(EXIT_ERR);
  }
  else
  {
    return (child_pid);
  }
}

static void print_proc_exit(int status)
{
  int exit_code = WEXITSTATUS (status);
  if (WIFSIGNALED(status))
  {
    int signal = WTERMSIG(status);
    std::cerr << "Terminated by signal " << str_signal(signal) << "."
              << std::endl;
  }
  else
  {
    std::cerr << "Terminated with code " << exit_code << "." << std::endl;
  }
}

static void exit_max_ressources(std::string res, pid_t pid, size_t ret)
{
  kill(pid, SIGKILL);
  std::cerr << "Terminated after max-" << res << " reached." << std::endl;
  exit(ret);
}

static void wait_process(pid_t pid, size_t time_max, size_t mem_max)
{
  close(0);
  close(1);
  // close(2);

  int status = 0;
  int exited = 0;
  size_t time = 0;
  size_t mem = 0;

  do
  {
    if (waitpid(pid, &status, WNOHANG) != 0)
    {
      if (WIFSTOPPED(status) && WSTOPSIG(status) == SIGTRAP)
        syscall_jail(pid, &mem);

      exited = WIFEXITED(status) || WIFSIGNALED(status);
    }

    usleep(SLEEP_TIME * 1000);
    time += SLEEP_TIME;
  }
  while (time_max > time && mem_max > mem && !exited);

  if (time >= time_max)
    exit_max_ressources("time", pid, EXIT_TIME);
  else if (mem >= mem_max)
    exit_max_ressources("mem", pid, EXIT_MEM);
  else
    print_proc_exit(status);
}

void exec_jail(option_parser& options)
{
  pid_t pid = run(options.get_cmd());
  wait_process(pid, options.get_time(), options.get_memory());
}

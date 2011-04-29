#include <iostream>
#include <cstdlib>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "const.hh"
#include "option_parser.hh"
#include "print.hh"
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

static void wait_process(pid_t pid, size_t time_max)
{
  close(0);
  close(1);
  // close(2);

  int status = 0;
  size_t time = 0;
  while (waitpid(pid, &status, WNOHANG) == 0 && time_max > time)
  {
    usleep(SLEEP_TIME);
    time += SLEEP_TIME;
  }

  // Max time reached
  if (time >= time_max)
  {
    kill(pid, SIGKILL);
    std::cerr << "Terminated after max-time reached." << std::endl;
    exit(EXIT_TIME);
  }
  // Normal exit
  else
  {
    print_proc_exit(status);
  }
}

void exec_jail(option_parser& options)
{
  pid_t pid = run(options.get_cmd());
  wait_process(pid, options.get_time());
}

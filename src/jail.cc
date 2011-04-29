#include "jail.hh"

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

jail::jail(const jail::cmd_type& cmd) : _cmd(cmd)
{
}

int jail::run()
{
  if ((_child_pid = fork()) == -1)
    return 1; // TODO: exception here
  else if (!_child_pid)
    exit(child_run());
  else
    return tracer_run();
}

int jail::child_run()
{
  const char* argv[_cmd.size() + 1];
  unsigned int i = 0;

  for (; i < _cmd.size(); ++i)
    argv[i] = _cmd[i].c_str();
  argv[i] = NULL;

  ptrace(PTRACE_TRACEME, 0, NULL, NULL);
  execvp(_cmd[0].c_str(), (char* const*)argv);

  return 1;
}

int jail::tracer_handle_status(int status, int& signum)
{
  if (WIFEXITED(status))
    return WEXITSTATUS(status);
  else if (WIFSIGNALED(status))
    return 1; // TODO: report an error
  else if (WSTOPSIG(status) != SIGTRAP)
    signum = WSTOPSIG(status);

  return -1;
}

int jail::tracer_run()
{
  while (true)
  {
    int status;
    int return_code;
    int signum = 0;

    if (waitpid(_child_pid, &status, 0) < 0)
    {
      // TODO: exception here
      return 1;
    }

    if ((return_code = tracer_handle_status(status, signum)) >= 0)
      return return_code;

    ptrace(PTRACE_SYSCALL, _child_pid, NULL, reinterpret_cast<void*>(signum));
  }
}

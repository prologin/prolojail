#include "jail.hh"

#include <sys/types.h>
#include <sys/wait.h>

jail::jail(const jail::cmd_type& cmd) : _cmd(cmd)
{
}

void jail::run()
{
  if ((_child_pid = fork()) == -1)
    return; // TODO: exception here
  else if (!_child_pid)
    child_run();
  else
    tracer_run();
}

void jail::child_run()
{
  const char* argv[_cmd.size() + 1];
  unsigned int i = 0;

  for (; i < _cmd.size(); ++i)
    argv[i] = _cmd[i].c_str();
  argv[i] = NULL;

  execvp(_cmd[0].c_str(), (char* const*)argv);
  // TODO: exception here
}

void jail::tracer_run()
{
  int status;
  waitpid(_child_pid, &status, 0);
}

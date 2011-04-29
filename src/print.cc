#include <vector>
#include <boost/lexical_cast.hpp>
#include "print.hh"

#define SIG_ADD_VECT(Name, Num, Desc) {         \
    name[Num] = Name;                           \
    desc[Num] = Desc;                           \
  }

std::string str_signal(size_t signal)
{
  std::vector<std::string> name;
  std::vector<std::string> desc;

  // name[0] and desc[0] are unused
  name.resize(20);
  desc.resize(name.size());

  SIG_ADD_VECT("SIGHUP", 1,
               "Hangup detected on controlling terminal or death of"
               "controlling process");
  SIG_ADD_VECT("SIGINT", 2, "Interrupt from keyboard");
  SIG_ADD_VECT("SIGQUIT", 3, "Quit from keyboard");
  SIG_ADD_VECT("SIGILL", 4, "Illegal Instruction");
  SIG_ADD_VECT("SIGTRAP", 5, "Trace/breakpoint trap");
  SIG_ADD_VECT("SIGABRT", 6, "Abort signal from abort(3)");
  SIG_ADD_VECT("SIGBUS", 7, "Bus error");
  SIG_ADD_VECT("SIGFPE", 8, "Floating point exception");
  SIG_ADD_VECT("SIGKILL", 9, "Kill signal");
  SIG_ADD_VECT("SIGUSR1", 10, "User-defined signal 1");
  SIG_ADD_VECT("SIGSEGV", 11, "Invalid memory reference");
  SIG_ADD_VECT("SIGUSR2", 12, "User-defined signal 2");
  SIG_ADD_VECT("SIGPIPE", 13, "Broken pipe: write to pipe with no readers");
  SIG_ADD_VECT("SIGALRM", 14, "Timer signal from alarm(2)");
  SIG_ADD_VECT("SIGTERM", 15, "Termination signal");
  SIG_ADD_VECT("SIGUSR1", 16, "User-defined signal 1");
  SIG_ADD_VECT("SIGUSR2", 17, "User-defined signal 2");
  SIG_ADD_VECT("SIGCONT", 18, "Continue if stopped");
  SIG_ADD_VECT("SIGSTOP", 19, "Stop process");

  if (signal > 0 && signal < name.size())
    return (name[signal] + " [" + desc[signal] + "]");

  return boost::lexical_cast<std::string>(signal);
}

#include "print.hh"

#include <boost/lexical_cast.hpp>
#include <map>
#include <vector>

typedef std::map<int, std::pair<std::string, std::string>> sig_descr_t;

const sig_descr_t signals =
{
  { 1, { "SIGHUP", "Hangup detected" }},
  { 2, { "SIGINT", "Program interrupted" }},
  { 3, { "SIGQUIT", "Program quitted" }},
  { 4, { "SIGILL", "Illegal instruction" }},
  { 5, { "SIGTRAP", "Breakpoint trap reached" }},
  { 6, { "SIGABRT", "Program aborted" }},
  { 7, { "SIGBUS", "Bus error (invalid memory access)" }},
  { 8, { "SIGFPE", "Floating point exception (division by zero?)" }},
  { 9, { "SIGKILL", "Program killed" }},
  { 10, { "SIGUSR1", "User-defined signal received" }},
  { 11, { "SIGSEGV", "Segmentation fault" }},
  { 12, { "SIGUSR2", "User-defined signal received" }},
  { 13, { "SIGPIPE", "Broken pipe" }},
  { 14, { "SIGALRM", "Timer triggered" }},
  { 15, { "SIGTERM", "Program terminated" }},
  { 16, { "SIGUSR1", "User-defined signal received" }},
  { 17, { "SIGUSR2", "User-defined signal received" }},
  { 18, { "SIGCONT", "Program continued" }},
  { 19, { "SIGSTOP", "Program stopped" }}
};

std::string str_signal(size_t signal)
{
  sig_descr_t::const_iterator it = signals.find(signal);
  if (it != signals.end())
  {
    return it->second.first + " (" + it->second.second + ")";
  }
  else
  {
    std::string s = boost::lexical_cast<std::string>(signal);
    return std::string("Unknown signal (code ") + s + ")";
  }
}

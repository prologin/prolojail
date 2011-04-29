#include <iostream>
#include <sys/ptrace.h>
#include "syscall_jail.hh"

void syscall_jail(pid_t pid, size_t* mem)
{
  (void) pid;
  (void) mem;

  std::cerr << "Jail called (syscall)!" << std::endl;
  ptrace (PTRACE_SYSCALL, pid, 0, 0);
}

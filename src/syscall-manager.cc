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

#include "syscall-manager.hh"
#include "const.hh"

#include <iostream>
#include <map>

#include <sys/user.h>
#include <sys/ptrace.h>
#include <asm/unistd_32.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

typedef std::map<int, std::string> t_syscall_name;

const t_syscall_name syscalls =
{
  { __NR_access, "access" }, // FIXME: to filter (pathname)
  { __NR_brk, "brk" },
  { __NR_close, "close" },
  { __NR_execve, "execve" },
  { __NR_exit_group, "exit_group" },
  { __NR_fstat64, "fstat64" },
  { __NR_mmap2, "mmap2" },
  { __NR_mprotect, "mprotect" },
  { __NR_munmap, "munmap" },
  { __NR_nanosleep, "nanosleep" },
  { __NR_open, "open" }, // FIXME: to filter (pathname)
  { __NR_read, "read" },
  { __NR_rt_sigaction, "rt_sigaction" },
  { __NR_rt_sigprocmask, "rt_sigprocmask" },
  { __NR_set_thread_area, "set_thread_area" },
  { __NR_stat64, "stat64" }, // FIXME: to filter (pathname)
  { __NR_write, "write" }
};

syscall_manager::syscall_manager() : in_syscall_(false), execve_passed_(false)
{
}

bool syscall_manager::is_allowed(int num) const
{
  t_syscall_name::const_iterator it = syscalls.find(num);
  return (it != syscalls.end());
}

int syscall_manager::handle()
{
  t_regs regs;
  ptrace(PTRACE_GETREGS, pid_, NULL, &regs);
  in_syscall_ = !in_syscall_;

  if (in_syscall_)
  {
    int num = regs.orig_eax;
    if (!is_allowed(num) || !is_deep_allowed(&regs, num))
    {
      std::cerr << "Syscall forbidden or blocked (num = " << num << ")"
                << std::endl;

#ifdef DEBUG
      std::cerr << "See /usr/include/asm-generic/unistd.h for more information"
                << std::endl << std::endl;
      print_call(&regs);
#endif

      kill(pid_, SIGKILL);
      return (0);
    }
  }

  return (1);
}

void syscall_manager::print_call(t_regs* regs)
{
    t_syscall_name::const_iterator it = syscalls.find(regs->orig_eax);
    std::string name;
    name = (it != syscalls.end()) ? it->second : "[unknown syscall]";

    std::cerr << "Syscall called: " << name << std::endl;
    std::cerr << "Arg1: " << regs->ebx << std::endl;
    std::cerr << "Arg2: " << regs->ecx << std::endl;
    std::cerr << "Arg3: " << regs->edx << std::endl;
    std::cerr << "Arg4: " << regs->esi << std::endl;
    std::cerr << "Arg5: " << regs->edi << std::endl;
    std::cerr << "Arg6: " << regs->ebp << std::endl;
    std::cerr << std::endl;
}

bool syscall_manager::is_deep_allowed(t_regs* regs, int sys)
{
  if (sys == __NR_execve)
    return (handle_sys_execve(regs));
  if (sys == __NR_open)
    return (handle_sys_open(regs));
  if (sys == __NR_access)
    return (handle_sys_access(regs));
  if (sys == __NR_stat64)
    return (handle_sys_stat64(regs));
  else
    return (true);
}

bool syscall_manager::handle_sys_execve(t_regs* regs)
{
  (void) regs;
  if (execve_passed_)
    return (false);
  else
  {
    execve_passed_ = true;
    return (true);
  }
}

/**
 * Forbid open on unauthorized paths and prevent file opening in write mode.
 */
bool syscall_manager::handle_sys_open(t_regs* regs)
{
  int flags = regs->ecx;
  if (flags != O_RDONLY)
    return (false);

  std::string file = get_process_string(regs->ebx);

#ifdef DEBUG
  std::cerr << "File opened: " << file << std::endl;
#endif

  return (path_allowed(file));
}

bool syscall_manager::handle_sys_access(t_regs* regs)
{
  // FIXME: check path
  (void) regs;
  return (true);
}

bool syscall_manager::handle_sys_stat64(t_regs* regs)
{
  // FIXME: check path
  (void) regs;
  return (true);
}

std::string syscall_manager::get_process_string(long addr)
{
  std::string s;
  for (char c; (c = ptrace(PTRACE_PEEKDATA, pid_, addr, 0)) != 0; addr++)
    s += c;
  return s;
}

bool syscall_manager::path_allowed(std::string& path)
{
  if (path == "/etc/ld.so.cache")
    return (true);
  else if (path.substr(0, 5) == "/lib/")
    return (true);
  else
    return (false);
}

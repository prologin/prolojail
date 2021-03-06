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

#ifndef SYSCALL_MANAGER_HH_
# define SYSCALL_MANAGER_HH_

# include <sys/types.h>
# include <boost/optional.hpp>

class syscall_manager
{
public:
  typedef struct user_regs_struct t_regs;

  syscall_manager();
  int handle();
  void set_pid(pid_t pid)
  {
    pid_ = pid;
  }
  bool is_allowed(int num) const;

protected:
  bool in_syscall_;
  pid_t pid_;
  bool execve_passed_;

  void print_call(t_regs* regs);
  bool is_deep_allowed(t_regs* regs, int sys);
  bool path_allowed(std::string& path);
  std::string get_process_string(long addr);

  bool handle_sys_execve(t_regs* regs);
  bool handle_sys_open(t_regs* regs);
  bool handle_sys_access(t_regs* regs);
  bool handle_sys_stat64(t_regs* regs);
};

#endif /* !SYSCALL_MANAGER_HH_ */

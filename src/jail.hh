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

#ifndef JAIL_HH_
# define JAIL_HH_

#include "exec-exception.hh"

# include <boost/optional.hpp>
# include <string>
# include <vector>

# include <time.h>

# define SLEEP_TIME_MS 100
# define ERR_PROCESS_TERM 1
# define ERR_MAX_TIME 2
# define ERR_MAX_MEM 4

class jail
{
public:
  typedef std::vector<std::string> cmd_type;

  jail(const cmd_type& cmd);
  int run();

  const cmd_type& cmd() const
  {
    return cmd_;
  }

  boost::optional<size_t>& time_limit()
  {
    return time_limit_;
  }

  const boost::optional<size_t>& time_limit() const
  {
    return time_limit_;
  }

  boost::optional<size_t>& memory_limit()
  {
    return mem_limit_;
  }

  const boost::optional<size_t>& memory_limit() const
  {
    return mem_limit_;
  }

private:
  int child_run();
  int tracer_run();
  int kill_process();

  size_t check_limits();
  size_t check_time_limit();
  size_t check_time_limit_fallback();
  size_t check_memory_limit();

  int handle_return_code(size_t res_thread, int return_code, int signum);

  void start_watchdog(pthread_t* thread);
  static void *thread_entry(void* obj);
  void* thread_run();

  int tracer_handle_status(int status, int& signum);

  cmd_type cmd_;
  boost::optional<size_t> time_limit_;
  boost::optional<size_t> mem_limit_;

  time_t timestamp_start_;
  pid_t child_pid_;
};

#endif // !JAIL_HH_

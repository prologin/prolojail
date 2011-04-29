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

#include "signal.hh"

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

std::string signal_to_string(int signum)
{
  sig_descr_t::const_iterator it = signals.find(signum);
  if (it != signals.end())
  {
    return it->second.first + " (" + it->second.second + ")";
  }
  else
  {
    std::string s = boost::lexical_cast<std::string>(signum);
    return std::string("Unknown signal (code ") + s + ")";
  }
}

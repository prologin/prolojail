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

#include "option_parser.hh"

#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <cstdlib>
#include <iostream>

void option_parser::parse(int argc, const char** argv)
{
  if (argc > 1 &&
      ("-h" == std::string(argv[1]) || std::string(argv[1]) == "--help"))
    print_usage(argv[0], 0, std::cout);

  if (argc < 4)
  {
    std::cerr << "Missing arguments!" << std::endl;
    print_usage(argv[0], 1, std::cerr);
  }

  try
  {
    int tmp;

    tmp = boost::lexical_cast<int>(argv[1]);
    _memory = boost::numeric_cast<size_t>(tmp);

    tmp = boost::lexical_cast<int>(argv[2]);
    _time = boost::numeric_cast<size_t>(tmp);
  }
  catch (boost::bad_lexical_cast& e)
  {
    std::cerr << "Memory/time must be integers!" << std::endl;
    print_usage(argv[0], 1, std::cerr);
  }
  catch (boost::bad_numeric_cast& e)
  {
    std::cerr << "Memory/time must be positive integers!" << std::endl;
    print_usage(argv[0], 1, std::cerr);
  }

  for (int i = 3; i < argc; ++i)
    _cmd.push_back(argv[i]);
}

void option_parser::print_usage(const std::string& prog, size_t ret,
                                std::ostream& out) const
{
  out << "Usage: " << prog << " <memory> <time> <bin> [args...]"
      << std::endl
      << "Execute the program BIN in a jail with MEMORY ko and TIME ms."
      << std::endl;

  exit(ret);
}

const option_parser::cmd_type& option_parser::get_cmd() const
{
  return _cmd;
}

size_t option_parser::get_memory() const
{
  return _memory;
}

size_t option_parser::get_time() const
{
  return _time;
}

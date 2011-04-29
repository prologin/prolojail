#include <iostream>
#include <cstdlib>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "option-parser.hh"

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

void option_parser::print_usage(std::string prog, size_t ret,
				std::ostream& out) const
{
  out << "Usage: " << prog << " <memory> <time> <bin> [args...]"
      << std::endl
      << "Execute the program BIN in a jail with MEMORY ko and TIME ms."
      << "" << std::endl;

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

#include "exec-exception.hh"

exec_exception::exec_exception (const std::string& message)
  : std::runtime_error(message)
{
}

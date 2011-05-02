#ifndef EXEC_EXCEPTION_HH_
# define EXEC_EXCEPTION_HH_

# include <stdexcept>

class exec_exception : public std::runtime_error
{
public:
  exec_exception (const std::string& message);
};

#endif /* !EXEC_EXCEPTION_HH_ */

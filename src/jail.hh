#ifndef JAIL_HH_
# define JAIL_HH_

# include <boost/optional.hpp>
# include <string>
# include <vector>

class jail
{
public:
  typedef std::vector<std::string> cmd_type;

  jail(const cmd_type& cmd);
  void run();

  const cmd_type& cmd() const
  {
    return _cmd;
  }

  boost::optional<size_t>& time_limit()
  {
    return _time_limit;
  }

  const boost::optional<size_t>& time_limit() const
  {
    return _time_limit;
  }

  boost::optional<size_t>& memory_limit()
  {
    return _mem_limit;
  }

  const boost::optional<size_t>& memory_limit() const
  {
    return _mem_limit;
  }

private:
  cmd_type _cmd;
  boost::optional<size_t> _time_limit;
  boost::optional<size_t> _mem_limit;
};

#endif // !JAIL_HH_

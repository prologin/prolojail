#ifndef OPTION_PARSER_HH_
# define OPTION_PARSER_HH_

# include <string>
# include <vector>

class option_parser
{
public:
  typedef std::vector<std::string> cmd_type;

  void parse(int argc, const char** argv);

  const cmd_type& get_cmd() const;
  size_t get_memory() const;
  size_t get_time() const;

protected:
  void print_usage(std::string prog, size_t ret, std::ostream& out) const;

  cmd_type _cmd;
  size_t _memory;
  size_t _time;
};

#endif /* !OPTION_PARSER_HH_ */

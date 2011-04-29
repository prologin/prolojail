#ifndef OPTION_PARSER_HH_
# define OPTION_PARSER_HH_

class option_parser
{
public:
  void parse(int argc, const char** argv);

  const char** get_cmd() const;
  size_t get_cmd_len() const;
  size_t get_memory() const;
  size_t get_time() const;

protected:
  bool file_exists(std::string filename);
  void print_usage(std::string prog, size_t ret, std::ostream& out) const;

  const char** _cmd;
  size_t _cmd_len;
  size_t _memory;
  size_t _time;
};

#endif /* !OPTION_PARSER_HH_ */

#include <unistd.h>

int main(void)
{
  char buf[256];
  int len;

  while ((len = read(STDIN_FILENO, buf, sizeof (buf))) > 0)
    write(STDOUT_FILENO, buf, len);

  return (0);
}

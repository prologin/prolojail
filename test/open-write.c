#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
  open("/lib/toto", O_WRONLY);
  return (0);
}

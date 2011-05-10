#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
  open("/tmp/toto", O_WRONLY);
  return (0);
}

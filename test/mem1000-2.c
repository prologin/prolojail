#include <unistd.h>
#include <string.h>

int main(void)
{
  char a[1024 * 1024];
  memset(a, 42, 1024 * 1024);

  sleep(1);
  return (0);
}

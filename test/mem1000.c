#include <unistd.h>
#include <stdlib.h>

int main(void)
{
  char *a = malloc(1000 * 1000);
  (void) a;
  sleep(1);
  return (0);
}

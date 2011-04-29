#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(void)
{
  kill(getpid(), SIGSEGV);
  return (0);
}

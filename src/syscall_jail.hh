#ifndef SYSCALL_JAIL_HH_
# define SYSCALL_JAIL_HH_

# include <cstdlib>

void syscall_jail(pid_t pid, size_t* mem);

#endif /* !SYSCALL_JAIL_HH_ */

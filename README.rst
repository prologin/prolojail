Prolojail
=========

Prolojail is a sandboxing system used by Prologin to execute untrusted code
sent by the contestants to check if their algorithm gives the correct output
for some given input.

The security comes from three major elements:

- A strict time limit is set for the program execution, avoiding infinite loops
  and slow problem solutions.

- A memory limit established by watching ``brk``/``sbrk``/``mmap`` syscalls to
  compute the currently used heap size.

- A syscall whitelist to forbid execution of other programs, network activity,
  file reading/writing in strange places (``/etc`` for example) and a lot of
  other things which could be achieved via syscalls.

This system is released under the BSD license.

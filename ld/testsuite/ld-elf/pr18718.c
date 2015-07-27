#include <stdio.h>
#include <bfd_stdint.h>

extern void foo (void);

void
new_foo (void)
{
}

__asm__(".symver new_foo, foo@@VERS_2.0");

__attribute__ ((noinline, noclone))
int
bar (void)
{
  return (intptr_t) &foo == 0x12345678 ? 1 : 0;
}

int
main(void)
{
  bar ();
  printf("PASS\n");
  return 0;
}
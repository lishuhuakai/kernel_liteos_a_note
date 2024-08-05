#include <sys/prctl.h>
#include <stdarg.h>
#include "syscall.h"

#ifdef HOOK_ENABLE
int __libc_prctl(int op,  ...);
#endif

#ifdef HOOK_ENABLE
int __libc_prctl(int op,  ...)
#else
int prctl(int op, ...)
#endif
{
	unsigned long x[4];
	int i;
	va_list ap;
	va_start(ap, op);
	for (i=0; i<4; i++) x[i] = va_arg(ap, unsigned long);
	va_end(ap);
	return syscall(SYS_prctl, op, x[0], x[1], x[2], x[3]);
}

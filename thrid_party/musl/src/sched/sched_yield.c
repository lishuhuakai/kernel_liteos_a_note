#include <sched.h>
#include "syscall.h"

int sched_yield()
{
#ifdef __LITEOS_A__
	return syscall(SYS_sched_yield, 0);
#else
	return syscall(SYS_sched_yield);
#endif
}

#include <threads.h>
#include "syscall.h"

void thrd_yield()
{
#ifdef __LITEOS_A__
	__syscall(SYS_sched_yield, -1);
#else
	__syscall(SYS_sched_yield);
#endif
}

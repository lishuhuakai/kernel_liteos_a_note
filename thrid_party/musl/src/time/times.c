#include <sys/times.h>
#include "syscall.h"

clock_t times(struct tms *tms)
{
#ifdef __LITEOS_A__
	return syscall(SYS_times, tms);
#else
	return __syscall(SYS_times, tms);
#endif
}

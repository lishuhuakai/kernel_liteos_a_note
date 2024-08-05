#include <sched.h>
#include <errno.h>
#include "syscall.h"
#ifdef __LITEOS_A__
#include "pthread_impl.h"
#endif

int sched_getscheduler(pid_t pid)
{
	int r = -ENOSYS;
#ifdef __LITEOS_A__
	r = __syscall(SYS_sched_getscheduler, pid, MUSL_TYPE_PROCESS);
#else
	r = __syscall(SYS_sched_getscheduler, pid);
#endif
	return __syscall_ret(r);
}

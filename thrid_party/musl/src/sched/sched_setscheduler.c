#include <sched.h>
#include <errno.h>
#include "syscall.h"
#ifdef __LITEOS_A__
#include "pthread_impl.h"
#endif

int sched_setscheduler(pid_t pid, int sched, const struct sched_param *param)
{
	int r;
	if (!param) {
		r = -EINVAL;
		goto exit;
	}
#ifdef __LITEOS_A__
	r = __syscall(SYS_sched_setscheduler, pid, sched, param, MUSL_TYPE_PROCESS);
#else
	r = __syscall(SYS_sched_setscheduler, pid, sched, param);
#endif
exit:
	return __syscall_ret(r);
}

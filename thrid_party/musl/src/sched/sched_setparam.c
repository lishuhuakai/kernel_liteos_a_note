#include <sched.h>
#include <errno.h>
#include "syscall.h"
#ifdef __LITEOS_A__
#include "pthread_impl.h"
#endif

int sched_setparam(pid_t pid, const struct sched_param *param)
{
	int r;
	if (!param) {
		r = -EINVAL;
		goto exit;
	}
#ifdef __LITEOS_A__
	r = __syscall(SYS_sched_setparam, pid, param, MUSL_TYPE_PROCESS);
#else
	r = __syscall(SYS_sched_setparam, pid, param);
#endif
exit:
	return __syscall_ret(r);
}

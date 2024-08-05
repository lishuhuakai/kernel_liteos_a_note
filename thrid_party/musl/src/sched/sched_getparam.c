#include <sched.h>
#include <errno.h>
#include "syscall.h"
#include <string.h>
#ifdef __LITEOS_A__
#include "pthread_impl.h"
#endif

int sched_getparam(pid_t pid, struct sched_param *param)
{
	int r;
	if (!param) {
		r = -EINVAL;
		goto exit;
	}
	memset(param, 0, sizeof(struct sched_param));
#ifdef __LITEOS_A__
	r = __syscall(SYS_sched_getparam, pid, param, MUSL_TYPE_PROCESS);
#else
	r = __syscall(SYS_sched_getparam, pid, param);
	if (r >= 0) {
		r = 0;
	}
#endif
exit:
	return __syscall_ret(r);
}

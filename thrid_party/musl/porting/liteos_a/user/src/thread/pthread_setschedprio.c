#include "pthread_impl.h"
#include "lock.h"

int pthread_setschedprio(pthread_t t, int prio)
{
	int r;
	struct sched_param param = {
		.sched_priority = prio,
	};

	sigset_t set;
	__block_app_sigs(&set);
	LOCK(t->killlock);
	r = !t->tid ? ESRCH : -__syscall(SYS_sched_setparam, t->tid, &param, MUSL_TYPE_THREAD);
	UNLOCK(t->killlock);
	__restore_sigs(&set);
	return r;
}

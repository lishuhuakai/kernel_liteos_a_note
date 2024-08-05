#include "pthread_impl.h"
#include "lock.h"

int pthread_setschedprio(pthread_t t, int prio)
{
	int r;
#ifdef __LITEOS_A__
	struct sched_param param = {
		.sched_priority = prio,
	};
#endif

	sigset_t set;
	__block_app_sigs(&set);
	LOCK(t->killlock);
#ifdef __LITEOS_A__
	r = !t->tid ? ESRCH : -__syscall(SYS_sched_setparam, t->tid, &param, MUSL_TYPE_THREAD);
#else
	r = !t->tid ? ESRCH : -__syscall(SYS_sched_setparam, t->tid, &prio);
#endif
	UNLOCK(t->killlock);
	__restore_sigs(&set);
	return r;
}

#include "pthread_impl.h"
#include <threads.h>

static int __pthread_detach(pthread_t t)
{
	/* If the cas fails, detach state is either already-detached
	 * or exiting/exited, and pthread_join will trap or cleanup. */
	if (a_cas(&t->detach_state, DT_JOINABLE, DT_DETACHED) != DT_JOINABLE) {
		int cs;
		__pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);
		__pthread_join(t, 0);
		__pthread_setcancelstate(cs, 0);
	}
#ifdef __LITEOS_A__
	int ret = __syscall(SYS_pthread_set_detach, t->tid);
	if (ret) {
		a_swap(&t->detach_state, DT_JOINABLE);
	}
	if (ret == ESRCH) {
		ret = 0;
	}
	return ret;
#else
	return 0;
#endif
}

weak_alias(__pthread_detach, pthread_detach);
weak_alias(__pthread_detach, thrd_detach);

#include "pthread_impl.h"

int pthread_attr_setschedpolicy(pthread_attr_t *a, int policy)
{
#ifdef __LITEOS_A__
	if (policy != SCHED_RR && policy != SCHED_FIFO && policy != SCHED_DEADLINE) {
		return EINVAL;
	}
#endif
	a->_a_policy = policy;
	return 0;
}

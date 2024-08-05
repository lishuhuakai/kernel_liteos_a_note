#include "pthread_impl.h"

int pthread_attr_init(pthread_attr_t *a)
{
	*a = (pthread_attr_t){0};
	__acquire_ptc();
	a->_a_stacksize = __default_stacksize;
	a->_a_guardsize = __default_guardsize;
#ifdef __LITEOS_A__
	a->_a_policy = SCHED_RR;
	a->_a_prio = 25;
#endif
	__release_ptc();
	return 0;
}

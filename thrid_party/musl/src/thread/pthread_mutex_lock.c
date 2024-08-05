#include "pthread_impl.h"

#ifndef __LITEOS__
extern int __pthread_mutex_timedlock_inner(pthread_mutex_t *restrict m, const struct timespec *restrict at);
#endif
int __pthread_mutex_lock(pthread_mutex_t *m)
{
	if ((m->_m_type&15) == PTHREAD_MUTEX_NORMAL
	    && !a_cas(&m->_m_lock, 0, EBUSY))
		return 0;

#ifndef __LITEOS__
	return __pthread_mutex_timedlock_inner(m, 0);
#else
	return __pthread_mutex_timedlock(m, 0);
#endif
}

weak_alias(__pthread_mutex_lock, pthread_mutex_lock);

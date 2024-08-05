#include "pthread_impl.h"

int pthread_mutex_init(pthread_mutex_t *restrict m, const pthread_mutexattr_t *restrict a)
{
	__builtin_memset(m, 0, sizeof(pthread_mutex_t));
	if (a) m->_m_type = a->__attr;
	return 0;
}

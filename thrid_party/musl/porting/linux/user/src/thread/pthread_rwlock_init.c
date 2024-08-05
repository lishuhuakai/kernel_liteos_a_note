#include "pthread_impl.h"

int pthread_rwlock_init(pthread_rwlock_t *restrict rw, const pthread_rwlockattr_t *restrict a)
{
	__builtin_memset(rw, 0, sizeof(pthread_rwlock_t));
	if (a) rw->_rw_shared = a->__attr[0]*128;
	return 0;
}

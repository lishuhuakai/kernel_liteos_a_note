#include "pthread_impl.h"

int pthread_cond_init(pthread_cond_t *restrict c, const pthread_condattr_t *restrict a)
{
	__builtin_memset(c, 0, sizeof(pthread_cond_t));
	if (a) {
		c->_c_clock = a->__attr & 0x7fffffff;
		if (a->__attr>>31) c->_c_shared = (void *)-1;
	}
	return 0;
}

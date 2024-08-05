#include "pthread_impl.h"
#include <unsupported_api.h>

int pthread_mutex_getprioceiling(const pthread_mutex_t *restrict m, int *restrict ceiling)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return EINVAL;
}

#include "pthread_impl.h"

int pthread_attr_setscope(pthread_attr_t *a, int scope)
{
	switch (scope) {
#ifdef __LITEOS_A__
	case PTHREAD_SCOPE_SYSTEM:
		return ENOTSUP;
	case PTHREAD_SCOPE_PROCESS:
		return 0;
#else
	case PTHREAD_SCOPE_SYSTEM:
		return 0;
	case PTHREAD_SCOPE_PROCESS:
		return ENOTSUP;
#endif
	default:
		return EINVAL;
	}
}

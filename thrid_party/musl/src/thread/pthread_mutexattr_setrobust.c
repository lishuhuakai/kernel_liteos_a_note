#include "pthread_impl.h"
#include "syscall.h"

#ifndef __LITEOS_A__
static volatile int check_robust_result = -1;
#endif

int pthread_mutexattr_setrobust(pthread_mutexattr_t *a, int robust)
{
	if (robust > 1U) return EINVAL;
	if (robust) {
#ifndef __LITEOS_A__
		int r = check_robust_result;
		if (r < 0) {
			void *p;
			size_t l;
			r = -__syscall(SYS_get_robust_list, 0, &p, &l);
			a_store(&check_robust_result, r);
		}
		if (r) return r;
#endif
		a->__attr |= 4;
		return 0;
	}
	a->__attr &= ~4;
	return 0;
}

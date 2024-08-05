#include "pthread_impl.h"
#include "pthread_ffrt.h"

int pthread_setspecific(pthread_key_t k, const void *x)
{
	struct pthread *self = __pthread_self();
	/* Avoid unnecessary COW */
	if (self->tsd[k] != x) {
		self->tsd[k] = (void *)x;
		self->tsd_used = 1;
	}
	return 0;
}

void pthread_settsd(void** tsd)
{
	struct pthread *self = __pthread_self();
	self->tsd = tsd;
}
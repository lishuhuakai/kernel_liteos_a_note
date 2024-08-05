#include "pthread_impl.h"

int __pthread_setcancelstate(int new, int *old)
{
#ifdef FEATURE_PTHREAD_CANCEL
	if (new > 2U) return EINVAL;
	struct pthread *self = __pthread_self();
	if (old) *old = self->canceldisable;
	self->canceldisable = new;
	return 0;
#endif
}

weak_alias(__pthread_setcancelstate, pthread_setcancelstate);

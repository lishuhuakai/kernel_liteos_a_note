#include "pthread_impl.h"

int pthread_setcanceltype(int new, int *old)
{
#ifdef FEATURE_PTHREAD_CANCEL
	struct pthread *self = __pthread_self();
	if (new > 1U) return EINVAL;
	if (old) *old = self->cancelasync;
	self->cancelasync = new;
	if (new) pthread_testcancel();
	return 0;
#endif
}

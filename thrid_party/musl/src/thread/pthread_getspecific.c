#include "pthread_impl.h"
#include "pthread_ffrt.h"
#include <threads.h>

static void *__pthread_getspecific(pthread_key_t k)
{
	struct pthread *self = __pthread_self();
	return self->tsd[k];
}

void** pthread_gettsd()
{
	struct pthread *self = __pthread_self();
	return self->tsd;
}

weak_alias(__pthread_getspecific, pthread_getspecific);
weak_alias(__pthread_getspecific, tss_get);

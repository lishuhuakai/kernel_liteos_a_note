#ifndef HOOK_ENABLE
#include <stdlib.h>

#ifdef USE_JEMALLOC
extern void je_free(void* p);
#endif

void free(void *p)
{
#ifdef USE_JEMALLOC
	return je_free(p);
#endif
	__libc_free(p);
}
#endif
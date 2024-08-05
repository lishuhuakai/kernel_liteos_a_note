#ifndef HOOK_ENABLE
#include <stdlib.h>
#ifdef USE_JEMALLOC
extern void* je_calloc(void* p, size_t newsize);
#endif
void *calloc(size_t m, size_t n)
{
#ifdef USE_JEMALLOC
	return je_calloc(m, n);
#endif
    return __libc_calloc(m, n);
}
#endif
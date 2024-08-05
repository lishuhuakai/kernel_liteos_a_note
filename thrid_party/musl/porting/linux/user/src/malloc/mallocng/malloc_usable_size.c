#include <stdlib.h>
#include "meta.h"

#ifdef USE_JEMALLOC
extern size_t je_malloc_usable_size(void *p);
#endif
#ifndef HOOK_ENABLE
size_t malloc_usable_size(void *p)
#else
size_t __libc_malloc_usable_size(void *p)
#endif
{
#ifdef USE_JEMALLOC
	return je_malloc_usable_size(p);
#endif
	if (!p) return 0;
	struct meta *g = get_meta(p);
	int idx = get_slot_index(p);
	size_t stride = get_stride(g);
	unsigned char *start = g->mem->storage + stride*idx;
	unsigned char *end = start + stride - IB;
	return get_nominal_size(p, end);
}

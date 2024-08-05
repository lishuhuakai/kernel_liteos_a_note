#ifdef HOOK_ENABLE
#include <unistd.h>
#include <sys/types.h>
#include "musl_malloc.h"
#include <malloc.h>
#include "musl_malloc_dispatch_table.h"
#include "common_def.h"
#include "musl_preinit_common.h"
#include "signal.h"

#define MALLOC_REPORT_LIMIT (300 * 1024 * 1024)

#ifdef USE_GWP_ASAN
extern void* libc_gwp_asan_malloc(size_t bytes);
extern void libc_gwp_asan_free(void *mem);
extern size_t libc_gwp_asan_malloc_usable_size(void *mem);
extern void* libc_gwp_asan_realloc(void *ptr, size_t size);
extern void* libc_gwp_asan_calloc(size_t nmemb, size_t size);
#endif

void* malloc(size_t bytes)
{
	if (bytes >= MALLOC_REPORT_LIMIT) {
		raise(MUSL_SIGNAL_LEAK_STACK);
	}
	volatile const struct MallocDispatchType* dispatch_table = (struct MallocDispatchType *)atomic_load_explicit(
		&__musl_libc_globals.current_dispatch_table, memory_order_acquire);
	if (__predict_false(dispatch_table != NULL)) {
		if (__get_memleak_hook_flag()) {
			return dispatch_table->malloc(bytes);
		}
		if (!__get_global_hook_flag()) {
#ifdef USE_GWP_ASAN
			return libc_gwp_asan_malloc(bytes);
#endif
			return MuslFunc(malloc)(bytes);
		}
		else if (!__get_hook_flag()) {
#ifdef USE_GWP_ASAN
			return libc_gwp_asan_malloc(bytes);
#endif
			return MuslFunc(malloc)(bytes);
		}
		return dispatch_table->malloc(bytes);
	}
#ifdef USE_GWP_ASAN
	return libc_gwp_asan_malloc(bytes);
#endif
	return  MuslFunc(malloc)(bytes);
}

void* aligned_alloc(size_t align, size_t len)
{
	volatile const struct MallocDispatchType* dispatch_table = (struct MallocDispatchType *)atomic_load_explicit(
		&__musl_libc_globals.current_dispatch_table, memory_order_acquire);
	if (__predict_false(dispatch_table != NULL)) {
		if (__get_memleak_hook_flag()) {
			return dispatch_table->aligned_alloc(align, len);
		}
		if (!__get_global_hook_flag()) {
			return MuslMalloc(aligned_alloc)(align, len);
		}
		else if (!__get_hook_flag()) {
			return MuslMalloc(aligned_alloc)(align, len);
		}
		return dispatch_table->aligned_alloc(align, len);
	}
	return  MuslMalloc(aligned_alloc)(align, len);
}

void free(void* mem)
{
	volatile const struct MallocDispatchType* dispatch_table = (struct MallocDispatchType *)atomic_load_explicit(
		&__musl_libc_globals.current_dispatch_table, memory_order_acquire);
	if (__predict_false(dispatch_table != NULL)) {
		if (__get_memleak_hook_flag()) {
			dispatch_table->free(mem);
			return;
		}
		if (!__get_global_hook_flag()) {
#ifdef USE_GWP_ASAN
			return libc_gwp_asan_free(mem);
#endif
			MuslFunc(free)(mem);
			return;
		}
		else if (!__get_hook_flag()) {
#ifdef USE_GWP_ASAN
			return libc_gwp_asan_free(mem);
#endif
			MuslFunc(free)(mem);
			return;
		}
		dispatch_table->free(mem);
		return;
	}
#ifdef USE_GWP_ASAN
	return libc_gwp_asan_free(mem);
#endif
	MuslFunc(free)(mem);
}

void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->mmap(addr, length, prot, flags, fd, offset);
	} else {
		return MuslMalloc(mmap)(addr, length, prot, flags, fd, offset);
	}
}

int munmap(void* addr, size_t length)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->munmap(addr, length);
	} else {
		return MuslMalloc(munmap)(addr, length);
	}
}

void* calloc(size_t m, size_t n)
{
	if ((m <= (UINT32_MAX / n)) && ((m * n) >= MALLOC_REPORT_LIMIT)) {
		raise(MUSL_SIGNAL_LEAK_STACK);
	}
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->calloc(m, n);
	} else {
#ifdef USE_GWP_ASAN
		return libc_gwp_asan_calloc(m, n);
#endif
		return MuslFunc(calloc)(m, n);
	}
}

void* realloc(void *p, size_t n)
{
	if (n >= MALLOC_REPORT_LIMIT) {
		raise(MUSL_SIGNAL_LEAK_STACK);
	}
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->realloc(p, n);
	} else {
#ifdef USE_GWP_ASAN
		return libc_gwp_asan_realloc(p, n);
#endif
		return MuslFunc(realloc)(p, n);
	}
}

size_t malloc_usable_size(void* addr)
{
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->malloc_usable_size(addr);
	} else {
#ifdef USE_GWP_ASAN
		return libc_gwp_asan_malloc_usable_size(addr);
#endif
		return MuslMalloc(malloc_usable_size)(addr);
	}
}

int prctl(int option, ...)
{
	unsigned long x[4];
	int i;
	va_list ap;
	va_start(ap, option);
	for (i=0; i<4; i++) x[i] = va_arg(ap, unsigned long);
	va_end(ap);
	volatile const struct MallocDispatchType* dispatch_table = get_current_dispatch_table();
	if (__predict_false(dispatch_table != NULL)) {
		return dispatch_table->prctl(option, x[0], x[1], x[2], x[3]);
	} else {
		return MuslMalloc(prctl)(option, x[0], x[1], x[2], x[3]);
	}
}

#endif

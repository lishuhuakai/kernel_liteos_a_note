#include <stdlib.h>
#include <stdint.h>
#include "libc.h"
#ifdef __LITEOS_A__
#include <signal.h>
#include <atomic.h>
#include <pthread.h>
#include "syscall.h"
#include <bits/errno.h>
#ifdef __LITEOS_DEBUG__
#include <stdbool.h>
#include <debug.h>
extern bool g_enable_check;
extern void mem_check_deinit(void);
extern void clean_recycle_list(bool clean_all);
#endif

pthread_mutex_t __exit_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
static void dummy()
{
}

/* atexit.c and __stdio_exit.c override these. the latter is linked
 * as a consequence of linking either __toread.c or __towrite.c. */
weak_alias(dummy, __funcs_on_exit);
weak_alias(dummy, __stdio_exit);
weak_alias(dummy, _fini);

extern weak hidden void (*const __fini_array_start)(void), (*const __fini_array_end)(void);

static void libc_exit_fini(void)
{
	uintptr_t a = (uintptr_t)&__fini_array_end;
	for (; a>(uintptr_t)&__fini_array_start; a-=sizeof(void(*)()))
		(*(void (**)())(a-sizeof(void(*)())))();
	_fini();
}

weak_alias(libc_exit_fini, __libc_exit_fini);

#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
extern void __cxa_thread_finalize();
#endif

_Noreturn void exit(int code)
{
#ifdef __LITEOS_A__
	sigset_t set;

	__block_app_sigs(&set);

	int ret = pthread_mutex_trylock(&__exit_mutex);
	if (ret == EBUSY) {
		pthread_exit(NULL);
	}

#ifdef __LITEOS_DEBUG__
	if (g_enable_check) {
		check_leak();
		check_heap_integrity();
		mem_check_deinit();
		clean_recycle_list(true);
	}
#endif
#endif

#if !defined(__LITEOS__) && !defined(__HISPARK_LINUX__)
	// Call thread_local dtors.
	__cxa_thread_finalize();
#endif
	__funcs_on_exit();
	__libc_exit_fini();
	__stdio_exit();
	_Exit(code);
}

#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <atomic.h>
#include <pthread.h>
#include "syscall.h"
#include "libc.h"
#include <bits/errno.h>

pthread_mutex_t __exit_mutex = PTHREAD_MUTEX_INITIALIZER;

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

_Noreturn void exit(int code)
{
	sigset_t set;

	__block_app_sigs(&set);

	int ret = pthread_mutex_trylock(&__exit_mutex);
	if (ret == EBUSY) {
		pthread_exit(NULL);
	}

	__funcs_on_exit();
	__libc_exit_fini();
	__stdio_exit();
	_Exit(code);
}

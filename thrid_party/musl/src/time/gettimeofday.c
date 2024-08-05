#include <time.h>
#include <sys/time.h>
#include "syscall.h"

#ifndef __LITEOS__
#include <time.h>
#include <errno.h>
#include <stdint.h>
#include "atomic.h"

#ifdef VDSO_GTD_SYM

static void *volatile vdso_gtd;

static int gtd_init(struct timeval *tv, void *tz)
{
	__get_vdso_info();
	void *p = __get_vdso_addr(VDSO_GTD_VER, VDSO_GTD_SYM);
	int (*f)(struct timeval *, void *) =
		(int (*)(struct timval *, void *))p;
	a_cas_p(&vdso_gtd, (void *)gtd_init, p);
	return f ? f(tv, tz) : -ENOSYS;
}

static void *volatile vdso_gtd = (void *)gtd_init;

#endif
#endif

int gettimeofday(struct timeval *restrict tv, void *restrict tz)
{
#if defined(VDSO_GTD_SYM) && (!defined(__LITEOS__))
	int r;
	int (*f)(struct timeval *, void *) =
	(int (*)(struct timeval *, void *))vdso_gtd;
	if (f) {
		r = f(tv, tz);
		if (!r) return r;
		if (r == -EINVAL) return __syscall_ret(r);
	}
#endif

	struct timespec ts;
	if (!tv) return 0;
	clock_gettime(CLOCK_REALTIME, &ts);
	tv->tv_sec = ts.tv_sec;
	tv->tv_usec = (int)ts.tv_nsec / 1000;
	return 0;
}

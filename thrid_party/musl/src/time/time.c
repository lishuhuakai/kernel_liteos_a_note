#include <time.h>
#include "syscall.h"
#ifndef __LITEOS__
#include <errno.h>
#include <stdint.h>
#include "atomic.h"

#ifdef VDSO_TIME_SYM

static void *volatile vdso_time;

static time_t time_init(time_t *t)
{
	__get_vdso_info();
	void *p = __get_vdso_addr(VDSO_TIME_VER, VDSO_TIME_SYM);
	time_t (*f)(time_t *) =
		(time_t (*)(time_t *))p;
	a_cas_p(&vdso_time, (void *)time_init, p);
	return f ? f(t) : -ENOSYS;
}

static void *volatile vdso_time = (void *)time_init;

#endif
#endif

time_t time(time_t *t)
{
#if defined(VDSO_TIME_SYM) && (!defined(__LITEOS__))
	time_t (*f)(time_t *) =
	(time_t (*)(time_t *))vdso_time;
    if (f) {
		return f(t);
	}
#endif

	struct timespec ts;
	__clock_gettime(CLOCK_REALTIME, &ts);
	if (t) *t = ts.tv_sec;
	return ts.tv_sec;
}

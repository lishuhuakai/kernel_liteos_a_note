#include <sys/resource.h>
#include <errno.h>
#include "syscall.h"
#include "libc.h"

#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define FIX(x) do{ if ((x)>=SYSCALL_RLIM_INFINITY) (x)=RLIM_INFINITY; }while(0)

#ifdef __LITEOS_A__
static int __setrlimit(int resource, const struct rlimit *rlim)
{
	unsigned long long k_rlim[2];
	struct rlimit tmp;
	if (SYSCALL_RLIM_INFINITY != RLIM_INFINITY) {
		tmp = *rlim;
		FIX(tmp.rlim_cur);
		FIX(tmp.rlim_max);
		rlim = &tmp;
	}

	k_rlim[0] = rlim->rlim_cur;
	k_rlim[1] = rlim->rlim_max;
	return __syscall(SYS_setrlimit, resource, k_rlim);
}
#endif

struct ctx {
#ifdef __LITEOS_A__
	const struct rlimit *rlim;
#else
	unsigned long lim[2];
#endif
	int res;
	int err;
};

#ifdef SYS_setrlimit
static void do_setrlimit(void *p)
{
	struct ctx *c = p;
	if (c->err>0) return;
#ifdef __LITEOS_A__
	c->err = -__setrlimit(c->res, c->rlim);
#else
	c->err = -__syscall(SYS_setrlimit, c->res, c->lim);
#endif
}
#endif

int setrlimit(int resource, const struct rlimit *rlim)
{
#ifdef __LITEOS_A__
	struct ctx c = { .res = resource, .rlim = rlim, .err = -1 };
#else
	struct rlimit tmp;
	if (SYSCALL_RLIM_INFINITY != RLIM_INFINITY) {
		tmp = *rlim;
		FIX(tmp.rlim_cur);
		FIX(tmp.rlim_max);
		rlim = &tmp;
	}
	int ret = __syscall(SYS_prlimit64, 0, resource, rlim, 0);
#ifdef SYS_setrlimit
	if (ret != -ENOSYS) return __syscall_ret(ret);

	struct ctx c = {
		.lim[0] = MIN(rlim->rlim_cur, MIN(-1UL, SYSCALL_RLIM_INFINITY)),
		.lim[1] = MIN(rlim->rlim_max, MIN(-1UL, SYSCALL_RLIM_INFINITY)),
		.res = resource, .err = -1
	};
	__synccall(do_setrlimit, &c);
	if (c.err) {
		if (c.err>0) errno = c.err;
		return -1;
	}
	return 0;
#else
	return __syscall_ret(ret);
#endif
#endif
}

weak_alias(setrlimit, setrlimit64);

#include <sys/resource.h>
#include <ulimit.h>
#include <stdarg.h>
#include <unsupported_api.h>
#ifndef __LITEOS__
#include <limits.h>
#endif

long ulimit(int cmd, ...)
{
	struct rlimit rl;

	UNSUPPORTED_API_VOID(LITEOS_A);
	getrlimit(RLIMIT_FSIZE, &rl);
	if (cmd == UL_SETFSIZE) {
		long val;
		va_list ap;
		va_start(ap, cmd);
		val = va_arg(ap, long);
		va_end(ap);
		rl.rlim_cur = 512ULL * val;
		if (setrlimit(RLIMIT_FSIZE, &rl)) return -1;
	}
#ifndef __LITEOS__
	return rl.rlim_cur == RLIM_INFINITY ? LONG_MAX : rl.rlim_cur / 512;
#else
	return rl.rlim_cur / 512;
#endif
}

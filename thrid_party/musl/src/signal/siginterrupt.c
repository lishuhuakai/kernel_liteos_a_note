#include <signal.h>
#include <unsupported_api.h>
#ifndef __LITEOS__
#include <errno.h>

extern int __libc_sigaction(int sig, const struct sigaction *restrict sa, struct sigaction *restrict old);
#endif
int siginterrupt(int sig, int flag)
{
#ifndef __LITEOS__
	if (sig - 32U < 3 || sig - 1U >= _NSIG-1) {
		errno = EINVAL;
		return -1;
	}
	struct sigaction sa;

	__libc_sigaction(sig, 0, &sa);
	if (flag) sa.sa_flags &= ~SA_RESTART;
	else sa.sa_flags |= SA_RESTART;

	return __libc_sigaction(sig, &sa, 0);
#else
	UNSUPPORTED_API_VOID(LITEOS_A);
	struct sigaction sa;

	sigaction(sig, 0, &sa);
	if (flag) sa.sa_flags &= ~SA_RESTART;
	else sa.sa_flags |= SA_RESTART;

	return sigaction(sig, &sa, 0);
#endif
}

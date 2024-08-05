#include <stdlib.h>
#include <signal.h>
#include "syscall.h"
#include "pthread_impl.h"
#include "atomic.h"
#include "lock.h"
#include "ksigaction.h"

_Noreturn void abort(void)
{
#ifdef __LITEOS_A__
	sigset_t set, pending;
	sigemptyset(&set);
	sigaddset(&set, SIGABRT);

	sigpending(&pending);
	__syscall(SYS_rt_sigprocmask, SIG_UNBLOCK, &set, 0, _NSIG / 8);
	if (!sigismember(&pending, SIGABRT)) {
		raise(SIGABRT);
	}
#else
	raise(SIGABRT);
#endif

	/* If there was a SIGABRT handler installed and it returned, or if
	 * SIGABRT was blocked or ignored, take an AS-safe lock to prevent
	 * sigaction from installing a new SIGABRT handler, uninstall any
	 * handler that may be present, and re-raise the signal to generate
	 * the default action of abnormal termination. */
	__block_all_sigs(0);
	LOCK(__abort_lock);
#ifdef __LITEOS_A__
	signal(SIGABRT, SIG_DFL);
#else
	__syscall(SYS_rt_sigaction, SIGABRT,
		&(struct k_sigaction){.handler = SIG_DFL}, 0, _NSIG/8);
#endif
	__syscall(SYS_tkill, __pthread_self()->tid, SIGABRT);
	__syscall(SYS_rt_sigprocmask, SIG_UNBLOCK,
		&(long[_NSIG/(8*sizeof(long))]){1UL<<(SIGABRT-1)}, 0, _NSIG/8);

	/* Beyond this point should be unreachable. */
	a_crash();
	raise(SIGKILL);
	_Exit(127);
}

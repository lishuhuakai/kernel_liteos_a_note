#include <signal.h>
#include <stdint.h>
#include "syscall.h"
#include "pthread_impl.h"
#include <unistd.h>

int raise(int sig)
{
	sigset_t set;
	__block_app_sigs(&set);
#ifdef __LITEOS__
        int ret = syscall(SYS_tkill, __pthread_self()->tid, sig);
#else
	int ret = syscall(SYS_tgkill, syscall(SYS_getpid), __pthread_self()->tid, sig);
#endif // #ifdef __LITEOS__
	__restore_sigs(&set);
	return ret;
}

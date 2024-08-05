#include <unistd.h>
#include "pthread_impl.h"
#include "syscall.h"

static pid_t __get_cached_pid()
{
	return __pthread_self()->pid;
}

pid_t getpid(void)
{
	pid_t pid = __get_cached_pid();
	if (pid != 0) {
		return pid;
	}
	return __syscall(SYS_getpid);
}

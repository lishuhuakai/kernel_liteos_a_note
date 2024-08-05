#include <unistd.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "musl_fdtrack_hook.h"
#endif


int pipe(int fd[2])
{
	int r = 0;
#ifdef SYS_pipe
	r = syscall(SYS_pipe, fd);
#else
	r = syscall(SYS_pipe2, fd, 0);
#endif

#ifdef OHOS_FDTRACK_HOOK_ENABLE
	if (!r) {
		FDTRACK_START_HOOK(fd[0]);
		FDTRACK_START_HOOK(fd[1]);
	}
#endif
	return r;
}

#include <sys/socket.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "musl_fdtrack_hook.h"
#endif

int accept(int fd, struct sockaddr *restrict addr, socklen_t *restrict len)
{
#ifdef OHOS_FDTRACK_HOOK_ENABLE
	return FDTRACK_START_HOOK(socketcall_cp(accept, fd, addr, len, 0, 0, 0));
#endif
	return socketcall_cp(accept, fd, addr, len, 0, 0, 0);
}

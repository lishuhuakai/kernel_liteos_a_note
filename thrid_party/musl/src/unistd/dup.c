#include <unistd.h>
#include "syscall.h"
#ifdef OHOS_FDTRACK_HOOK_ENABLE
#include "musl_fdtrack_hook.h"
#endif

int dup(int fd)
{
#ifdef OHOS_FDTRACK_HOOK_ENABLE
	return FDTRACK_START_HOOK(syscall(SYS_dup, fd));
#endif
	return syscall(SYS_dup, fd);
}

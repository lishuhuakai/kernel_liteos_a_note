#include <sys/mman.h>
#include "syscall.h"
#include <unsupported_api.h>

int msync(void *start, size_t len, int flags)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return syscall_cp(SYS_msync, start, len, flags);
}

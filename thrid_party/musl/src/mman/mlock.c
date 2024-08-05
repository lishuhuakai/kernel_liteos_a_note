#include <sys/mman.h>
#include "syscall.h"
#include <unsupported_api.h>

int mlock(const void *addr, size_t len)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
#ifdef SYS_mlock
	return syscall(SYS_mlock, addr, len);
#else
	return syscall(SYS_mlock2, addr, len, 0);
#endif
}

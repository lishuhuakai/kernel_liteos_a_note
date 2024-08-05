#include <sys/mman.h>
#include "syscall.h"
#include <unsupported_api.h>

int munlock(const void *addr, size_t len)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return syscall(SYS_munlock, addr, len);
}

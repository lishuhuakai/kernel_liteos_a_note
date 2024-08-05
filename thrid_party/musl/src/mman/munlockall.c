#include <sys/mman.h>
#include "syscall.h"
#include <unsupported_api.h>

int munlockall(void)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return syscall(SYS_munlockall);
}

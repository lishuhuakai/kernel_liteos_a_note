#include <sys/mman.h>
#include "syscall.h"
#include <unsupported_api.h>

int mlockall(int flags)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return syscall(SYS_mlockall, flags);
}

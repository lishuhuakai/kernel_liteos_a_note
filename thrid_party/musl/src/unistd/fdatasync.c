#include <unistd.h>
#include "syscall.h"
#include <unsupported_api.h>

int fdatasync(int fd)
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return syscall_cp(SYS_fdatasync, fd);
}

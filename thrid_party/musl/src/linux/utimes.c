#include <sys/time.h>
#include "fcntl.h"
#include "syscall.h"
#include <unsupported_api.h>

int utimes(const char *path, const struct timeval times[2])
{
	UNSUPPORTED_API_VOID(LITEOS_A);
	return __futimesat(AT_FDCWD, path, times);
}

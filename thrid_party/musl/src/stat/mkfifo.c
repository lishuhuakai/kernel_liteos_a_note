#include <sys/stat.h>
#ifdef __LITEOS_A__
#include "syscall.h"
#endif

int mkfifo(const char *path, mode_t mode)
{
#ifdef __LITEOS_A__
	return syscall(SYS_mkfifo, path, mode);
#else
	return mknod(path, mode | S_IFIFO, 0);
#endif
}

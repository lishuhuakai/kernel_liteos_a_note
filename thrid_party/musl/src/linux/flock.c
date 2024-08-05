#include <sys/file.h>
#include "syscall.h"

#ifndef __LITEOS__
int __flock(int fd, int op)
#else
int flock(int fd, int op)
#endif
{
	return syscall(SYS_flock, fd, op);
}

#ifndef __LITEOS__
weak_alias(__flock, flock);
#endif

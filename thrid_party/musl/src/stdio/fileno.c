#include "stdio_impl.h"
#include <errno.h>

int fileno_unlocked(FILE *f)
{
	int fd = f->fd;
	if (fd < 0) {
		errno = EBADF;
		return -1;
	}
	return fd;
}

int fileno(FILE *f)
{
	FLOCK(f);
	int fd = fileno_unlocked(f);
	FUNLOCK(f);
	return fd;
}

#include "stdio_impl.h"
#include <fcntl.h>
#include <string.h>
#include <errno.h>

FILE *fopen(const char *restrict filename, const char *restrict mode)
{
	FILE *f = NULL;
	int fd = -1;
	int file_flags = 0;
	int mode_flags = 0;

	/* Compute the flags to pass to open() */
	mode_flags = __fmodeflags(mode, &file_flags);
	if (mode_flags < 0) {
		return NULL;
	}

	fd = sys_open(filename, mode_flags, 0666);
	if (fd < 0) return 0;
	if (mode_flags & O_CLOEXEC)
		__syscall(SYS_fcntl, fd, F_SETFD, FD_CLOEXEC);

	f = __fdopenx(fd, file_flags);
	if (f) {
		return f;
	}

	__syscall(SYS_close, fd);
	return 0;
}

weak_alias(fopen, fopen64);

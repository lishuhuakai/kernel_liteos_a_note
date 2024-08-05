#include "stdio_impl.h"
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int __fmodeflags(const char *mode, int *flags)
{
	int mode_flags = 0;
	int options = 0;

	switch (*mode) {
		case 'r':
			mode_flags = O_RDONLY;
			*flags = F_NOWR;
			break;
		case 'w':
			mode_flags = O_WRONLY;
			options = O_TRUNC | O_CREAT;
			*flags = F_NORD;
			break;
		case 'a':
			mode_flags = O_WRONLY;
			options = O_APPEND | O_CREAT;
			*flags = F_NORD | F_APP;
			break;
		default:
			errno = EINVAL;
			return -EINVAL;
	}

	mode++;
	while (*mode != '\0') {
		switch (*mode) {
			case '+':
				mode_flags = O_RDWR;
				*flags &= ~(F_NORD | F_NOWR);
				break;
			case 'x':
				/* need O_CREAT check */
				options |= O_EXCL;
				break;
			case 'e':
				options |= O_CLOEXEC;
				break;
			case 'b':
				break;
			default:
				/* only accept "+xeb" */
				break;
		}
		mode++;
	}

	return mode_flags | options;
}

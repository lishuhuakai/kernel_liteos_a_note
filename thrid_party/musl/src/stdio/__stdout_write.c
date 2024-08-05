#include "stdio_impl.h"
#include <sys/ioctl.h>

size_t __stdout_write(FILE *f, const unsigned char *buf, size_t len)
{
	struct winsize wsz;
	f->write = __stdio_write;
	/*
	 * write directly at first time.
	 * check f->flags & tty, and take effect later.
	 */
	size_t cnt = __stdio_write(f, buf, len);

	if (!(f->flags & F_SVB) && __syscall(SYS_ioctl, f->fd, TIOCGWINSZ, &wsz))
		f->lbf = -1;

	return cnt;
}

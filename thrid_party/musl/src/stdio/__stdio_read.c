#include "stdio_impl.h"
#include <sys/uio.h>

size_t __stdio_readx(FILE *f, unsigned char *buf, size_t len)
{
	ssize_t cnt = syscall(SYS_read, f->fd, buf, len);
	if (cnt <= 0) {
		f->flags |= cnt ? F_ERR : F_EOF;
		return 0;
	}
	return cnt;
}

size_t __stdio_read(FILE *f, unsigned char *buf, size_t len)
{
	struct iovec iov_buf[2] = {
		{ .iov_base = buf, .iov_len = len - !!f->buf_size },
		{ .iov_base = f->buf, .iov_len = f->buf_size }
	};
	ssize_t cnt;

	cnt = iov_buf[0].iov_len ? syscall(SYS_readv, f->fd, iov_buf, 2)
		: syscall(SYS_read, f->fd, iov_buf[1].iov_base, iov_buf[1].iov_len);
	if (cnt <= 0) {
		f->flags |= cnt ? F_ERR : F_EOF;
		return 0;
	}
	if (cnt <= iov_buf[0].iov_len) {
		return cnt;
	}
	cnt -= iov_buf[0].iov_len;
	f->rpos = f->buf;
	f->rend = f->buf + cnt;
	if (f->buf_size) buf[len-1] = *f->rpos++;
	return len;
}

/* Copyright (c) 2021-2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stdio_impl.h"
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "libc.h"

static size_t __get_bufsize(int fd)
{
	struct stat st;
	size_t buf_size = 0;

	if (fstat(fd, &st) < 0) {
		buf_size = BUFSIZ;
	} else if (st.st_blksize == 0) {
		buf_size = BUFSIZ;
	} else {
		buf_size = st.st_blksize;
	}

	return buf_size;
}

int __falloc_buf(FILE *f)
{
	/* return if already allocated, or F_NOBUF set */
	if (f->buf != NULL || f->buf_size != 0 || f->flags & F_NOBUF) {
		return 0;
	}

	/* Default,  base and buf are NULL,and buf_size = 0 */
	size_t buf_size = 0;

	/* get buffer size via file stat */
	buf_size = __get_bufsize(f->fd);

	/* alloc R/W buffer */
	f->base = (unsigned char *)malloc(UNGET + buf_size * sizeof(unsigned char));
	if (!f->base) {
		errno = -ENOMEM;
		return errno;
	}

	/* reserve UNGET buffer */
	f->buf = f->base + UNGET;
	f->buf_size = buf_size;

	return 0;
}

FILE *__fdopen(int fd, const char *mode)
{
	FILE *f = NULL;
	int file_flags = 0;
	int mode_flags = 0;

	int flags = syscall(SYS_fcntl, fd, F_GETFL, 0);
	if (flags == -1) {
		return NULL;
	}

	/* Compute the flags to pass to open() */
	mode_flags = __fmodeflags(mode, &file_flags);
	if (mode_flags < 0) {
		return NULL;
	}

	if (mode_flags & O_CLOEXEC) {
		__syscall(SYS_fcntl, fd, F_SETFD, FD_CLOEXEC);
	}

	if (mode_flags & O_APPEND) {
		flags = __syscall(SYS_fcntl, fd, F_GETFL);
		if (!(flags & O_APPEND))
			__syscall(SYS_fcntl, fd, F_SETFL, flags | O_APPEND);
	}

	f = __fdopenx(fd, file_flags);
	if (f) {
		return f;
	}

	return NULL;
}
weak_alias(__fdopen, fdopen);

FILE *__fdopenx(int fd, int flags)
{
	FILE *f = 0;
	struct winsize wsz;

	/* Allocate FILE or fail */
	if (!(f = __ofl_alloc())) {
		return NULL;
	}

	/* Zero-fill only the struct, not the buffer */
	memset(f, 0, sizeof *f);

	f->flags = flags;
	f->fd = fd;

	/* Activate line buffered mode for terminals */
	f->lbf = EOF;
	if (!(f->flags & F_NOWR) && !__syscall(SYS_ioctl, fd, TIOCGWINSZ, &wsz)) {
		f->lbf = '\n';
	}

	/* set file tag in fdtable, FILE type */
	fdsan_exchange_owner_tag(fd, 0, __get_file_tag(f));

	/* Initialize op ptrs. No problem if some are unneeded. */
	f->read = __stdio_read;
	f->write = __stdio_write;
	f->seek = __stdio_seek;
	f->close = __stdio_close;
	f->readx = __stdio_readx;

	if (!libc.threaded) {
		f->lock = -1;
	}

	/* Add new FILE to open file list */
	return __ofl_add(f);
}


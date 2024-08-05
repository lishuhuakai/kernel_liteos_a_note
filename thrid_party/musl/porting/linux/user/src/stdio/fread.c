#include "stdio_impl.h"
#include <string.h>
#include "param_check.h"

#define MIN(a,b) ((a)<(b) ? (a) : (b))

int __fill_buffer(FILE *f)
{
	int r = __toread(f);
	if (r != 0) {
		return r;
	}

	int k = f->readx(f, f->buf, f->buf_size);
	if (k <= 0) {
		f->flags |= (k == 0) ? F_EOF : F_ERR;
		f->rpos = f->rend;
		return k;
	}

	f->rpos = f->buf;
	f->rend = f->rpos + k;

	return 0;
}

size_t fread(void *restrict destv, size_t size, size_t nmemb, FILE *restrict f)
{
	unsigned char *dest = destv;
	size_t len = size * nmemb, l = len, k;
	PARAM_CHECK(f);
	if (!size) {
		nmemb = 0;
	}

	FLOCK(f);

	/* allocate file buffer if needed */
	if (__falloc_buf(f) < 0) {
		f->flags |= F_ERR;
		goto exit;
	}

	f->mode |= f->mode-1;

	while (l > 0) {
		if (f->rpos != f->rend) {
			/* First exhaust the buffer. */
			k = MIN(f->rend - f->rpos, l);
			memcpy(dest, f->rpos, k);
			f->rpos += k;
			dest += k;
			l -= k;
		}
		/* done */
		if (l == 0) {
			goto exit;
		}
		/* if user buffer is longer than file buffer,
		 * maybe buffer size is 0, non-buffer mode,
		 * read directly */
		if (l > f->buf_size) {
			break;
		}

		if (__fill_buffer(f)) {
			goto exit;
		}
	}

	/* Read the remainder directly */
	for (; l; l-=k, dest+=k) {
		k = f->readx(f, dest, l);
		if (!k) {
			break;
		}
	}

exit:
	FUNLOCK(f);
	return (len - l) / size;
}

weak_alias(fread, fread_unlocked);

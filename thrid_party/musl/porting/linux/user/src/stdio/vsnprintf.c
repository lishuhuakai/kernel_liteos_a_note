#include "stdio_impl.h"
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

struct cookie {
	char *s;
	size_t n;
};

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t sn_write(FILE *f, const unsigned char *s, size_t l)
{
	struct cookie *c = f->cookie;
	size_t already_size = f->wpos - f->wbase;
	if (already_size <= c->n) {
		size_t k = MIN(l, c->n - already_size);
		memcpy(f->wpos, s, k);
		f->wpos += k;
	}
	/* pretend to succeed, even if we discarded extra data */
	return l;
}

int vsnprintf(char *restrict s, size_t n, const char *restrict fmt, va_list ap)
{
	unsigned char dummy[1];
	struct cookie c = { .s = n ? s : dummy, .n = n ? n-1 : 0 };
	FILE f = {
		.lbf = EOF,
		.lock = -1,
		.buf = (unsigned char *)(n ? s: dummy),
		.buf_size = n ? n - 1 : 0,
		.flags = F_PBUF,
		.cookie = &c,
		.write = sn_write,
	};

	if (n > INT_MAX) {
		errno = EOVERFLOW;
		return -1;
	}

	return vfprintf(&f, fmt, ap);
}

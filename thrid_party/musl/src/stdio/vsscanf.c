#include "stdio_impl.h"
#include <string.h>

// Empty implementation because vfscanf operates rpos directly.
static size_t string_read(FILE *f, unsigned char *buf, size_t len)
{
	return 0;
}

int vsscanf(const char *restrict s, const char *restrict fmt, va_list ap)
{
	size_t s_len = strlen(s);
	FILE f = {
		.buf = (void *)s, .cookie = (void *)s,
		.read = string_read, .lock = -1,
		.buf_size = s_len, .rpos = s, .rend = s + s_len,
	};
	return vfscanf(&f, fmt, ap);
}

weak_alias(vsscanf,__isoc99_vsscanf);

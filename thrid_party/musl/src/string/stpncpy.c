#include <string.h>

char *__stpncpy(char *dst, const char *src, size_t n)
{
	char *d = dst, *s = src;
	dst = &dst[n];
	while (n > 0) {
		if ((*d++ = *src++) == 0) {
			dst = d - 1;
			while (--n > 0) {
				*d++ = 0;
			}
			break;
		}
		n--;
	}
	return (dst);
}

weak_alias(__stpncpy, stpncpy);

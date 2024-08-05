#include <string.h>

char *strncat(char *restrict d, const char *restrict s, size_t n)
{
	char *a = d;
	while (*d != 0) d++;
	while (n && *s) n--, *d++ = *s++;
	*d++ = 0;
	return a;
}

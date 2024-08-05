#include "stdio_impl.h"
#include "intscan.h"
#include "shgetc.h"
#include <inttypes.h>
#include <limits.h>
#include <ctype.h>

static unsigned long long strtox(const char *s, char **p, int base, unsigned long long lim)
{
	FILE f;
	sh_fromstring(&f, s);
	shlim(&f, 0);
	unsigned long long y = __intscan(&f, base, 1, lim);
	if (p) {
		size_t cnt = shcnt(&f);
		*p = (char *)s + cnt;
	}
	return y;
}

unsigned long long strtoull(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, ULLONG_MAX);
}

long long strtoll(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, LLONG_MIN);
}

#ifndef __LITEOS__
unsigned long strtoul_weak(const char *restrict s, char **restrict p, int base)
#else
unsigned long strtoul(const char *restrict s, char **restrict p, int base)
#endif
{
	return strtox(s, p, base, ULONG_MAX);
}

long strtol(const char *restrict s, char **restrict p, int base)
{
	return strtox(s, p, base, 0UL+LONG_MIN);
}

#ifndef __LITEOS__
intmax_t strtoimax_weak(const char *restrict s, char **restrict p, int base)
#else
intmax_t strtoimax(const char *restrict s, char **restrict p, int base)
#endif
{
	return strtoll(s, p, base);
}

#ifndef __LITEOS__
uintmax_t strtoumax_weak(const char *restrict s, char **restrict p, int base)
#else
uintmax_t strtoumax(const char *restrict s, char **restrict p, int base)
#endif
{
	return strtoull(s, p, base);
}

weak_alias(strtol, __strtol_internal);
#ifndef __LITEOS__
weak_alias(strtoul_weak, strtoul);
#endif
weak_alias(strtoul, __strtoul_internal);
weak_alias(strtoll, __strtoll_internal);
weak_alias(strtoull, __strtoull_internal);
#ifndef __LITEOS__
weak_alias(strtoimax_weak, strtoimax);
weak_alias(strtoumax_weak, strtoumax);
#endif
weak_alias(strtoimax, __strtoimax_internal);
weak_alias(strtoumax, __strtoumax_internal);

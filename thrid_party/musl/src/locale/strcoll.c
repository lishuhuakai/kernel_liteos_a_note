#include <string.h>
#include <locale.h>
#include "locale_impl.h"

int __strcoll_l(const char *l, const char *r, locale_t loc)
{
	return strcmp(l, r);
}

int strcoll(const char *l, const char *r)
{
#ifdef __LITEOS_A__
	return strcmp(l, r);
#else
	return __strcoll_l(l, r, CURRENT_LOCALE);
#endif
}

weak_alias(__strcoll_l, strcoll_l);

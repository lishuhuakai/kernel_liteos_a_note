#include "stdio_impl.h"
#include <string.h>
#ifndef __LITEOS__
#include "param_check.h"
#endif

int fputs(const char *restrict s, FILE *restrict f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	size_t l = strlen(s);
	return (fwrite(s, 1, l, f)==l) - 1;
}

weak_alias(fputs, fputs_unlocked);

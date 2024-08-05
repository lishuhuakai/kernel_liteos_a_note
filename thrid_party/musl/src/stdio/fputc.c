#include "stdio_impl.h"
#include <stdio.h>
#include "putc.h"
#ifndef __LITEOS__
#include "param_check.h"
#endif

int fputc(int c, FILE *f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	return do_putc(c, f);
}

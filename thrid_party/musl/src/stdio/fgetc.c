#include <stdio.h>
#include "getc.h"
#include "stdio_impl.h"
#ifndef __LITEOS__
#include "param_check.h"
#endif

int fgetc(FILE *f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	return do_getc(f);
}

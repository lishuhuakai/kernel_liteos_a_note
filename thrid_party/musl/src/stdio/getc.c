#include <stdio.h>
#include "getc.h"
#ifndef __LITEOS__
#include "param_check.h"
#endif

int getc(FILE *f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	return do_getc(f);
}

weak_alias(getc, _IO_getc);

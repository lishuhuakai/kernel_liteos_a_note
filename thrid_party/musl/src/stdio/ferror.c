#include "stdio_impl.h"
#ifndef __LITEOS__
#include "param_check.h"
#endif

#undef ferror

int ferror(FILE *f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	FLOCK(f);
	int ret = !!(f->flags & F_ERR);
	FUNLOCK(f);
	return ret;
}

weak_alias(ferror, ferror_unlocked);
weak_alias(ferror, _IO_ferror_unlocked);

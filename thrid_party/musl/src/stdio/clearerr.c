#include "stdio_impl.h"
#ifndef __LITEOS__
#include "param_check.h"
#endif

void clearerr(FILE *f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	FLOCK(f);
	f->flags &= ~(F_EOF|F_ERR);
	FUNLOCK(f);
}

weak_alias(clearerr, clearerr_unlocked);

#include "stdio_impl.h"
#ifndef __LITEOS__
#include "param_check.h"
#endif

#undef feof

int feof(FILE *f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	FLOCK(f);
	int ret = !!(f->flags & F_EOF);
	FUNLOCK(f);
	return ret;
}

weak_alias(feof, feof_unlocked);
weak_alias(feof, _IO_feof_unlocked);

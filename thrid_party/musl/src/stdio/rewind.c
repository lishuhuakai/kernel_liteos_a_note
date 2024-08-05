#include "stdio_impl.h"
#ifndef __LITEOS__
#include "param_check.h"
#endif

void rewind(FILE *f)
{
#ifndef __LITEOS__
	PARAM_CHECK(f);
#endif
	FLOCK(f);
	__fseeko_unlocked(f, 0, SEEK_SET);
	f->flags &= ~F_ERR;
	FUNLOCK(f);
}

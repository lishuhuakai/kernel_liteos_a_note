#include <dlfcn.h>
#include "dynlink.h"

#ifndef __LITEOS__
static int dummy(void *p)
{
	return __dl_invalid_handle(p);
}
weak_alias(dummy, __dlclose);
#endif

int dlclose(void *p)
{
#ifndef __LITEOS__
	return __dlclose(p);
#else
	return __dl_invalid_handle(p);
#endif
}

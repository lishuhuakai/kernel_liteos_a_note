#include "global.h"

__attribute__((weak)) int func()
{
	return LOCAL_VALUE;
}

int local_caller()
{
	return func();
}

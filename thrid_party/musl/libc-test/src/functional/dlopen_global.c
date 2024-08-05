#include "global.h"

__attribute__((weak)) int func()
{
	return GLOBAL_VALUE;
}

int global_caller()
{
	return func();
}

#include "dlopen_weak_deps.h"
#include "global.h"

__attribute__((weak)) int test_function()
{
	return GLOBAL_VALUE;
}

int test_number()
{
	return test_number2();
}

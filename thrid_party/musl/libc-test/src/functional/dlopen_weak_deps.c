#include "global.h"
#include "dlopen_weak_deps.h"

__attribute__((weak)) int test_function()
{
	return LOCAL_VALUE;
}

int test_number2()
{
	return test_function();
}

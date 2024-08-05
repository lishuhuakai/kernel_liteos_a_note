#include "dlopen_test_dependency.h"

__attribute__((weak)) int DlopenTestDependencyImpl(void)
{
    return 0;
}

int DlopenTestDependency(void)
{
    return DlopenTestDependencyImpl();
}

void DlopenTestDependency01() {}
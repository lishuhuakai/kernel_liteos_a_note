#include "dlopen_test_dependency.h"

int DlopenTestDependencyImpl(void)
{
    DlopenTestDependency01();
    return 1;
}
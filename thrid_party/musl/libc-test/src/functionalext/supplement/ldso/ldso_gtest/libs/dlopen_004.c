#include "dlopen_004.h"

void Dlopen004()
{
    Dlopen0041();
}

__attribute__((weak)) int DlopenTestImpl004()
{
    return 0;
}

int DlopenTest004()
{
    return DlopenTestImpl004();
}
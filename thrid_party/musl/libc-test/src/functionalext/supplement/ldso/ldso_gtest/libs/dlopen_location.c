#include "dlopen_relocation.h"
#include "dlopen_relocation_1.h"

int RelocationTest(void)
{
    RelocationTestOrder01();
    return RelocationTestOrder();
}

int RelocationMainProcessTest()
{
    return RelocationMainProcessTestImpl();
}

#include "dlopen_relocation.h"
#include "dlopen_relocation_1.h"
#include "dlopen_relocation_2.h"

int RelocationTest(void)
{
    RelocationTestOrder01();
    RelocationTestOrder02();
    return RelocationTestOrder();
}

int RelocationMainProcessTest()
{
    return RelocationMainProcessTestImpl();
}
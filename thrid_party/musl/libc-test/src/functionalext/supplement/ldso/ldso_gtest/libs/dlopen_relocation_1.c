#include "dlopen_relocation.h"
#include "dlopen_relocation_1.h"
#include "ldso_gtest_util.h"

int RelocationTestOrder(void)
{
    return EXPECT_RETURN_VALUE_15;
}

int RelocationMainProcessTestImpl(void)
{
    return EXPECT_RETURN_VALUE_45;
}

void RelocationTestOrder01() {}
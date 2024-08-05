#include "ldso_gtest_util.h"

__attribute__((weak)) int WeakFunc()
{
    return EXPECT_RETURN_VALUE_66;
}

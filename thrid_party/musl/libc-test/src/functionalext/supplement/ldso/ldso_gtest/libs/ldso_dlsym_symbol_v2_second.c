#include "ldso_gtest_util.h"

__asm__(".symver DlsymVersionV2,DlsymVersion@@DlsymVersion_2");
int DlsymVersionV2()
{
    return EXPECT_RETURN_VALUE_22;
}

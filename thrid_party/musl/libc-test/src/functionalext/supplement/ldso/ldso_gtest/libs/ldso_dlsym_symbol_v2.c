#include "ldso_gtest_util.h"

__asm__(".symver DlsymVersionV1,DlsymVersion@DlsymVersion_1");
int DlsymVersionV1()
{
    return 1;
}

__asm__(".symver DlsymVersionV2,DlsymVersion@@DlsymVersion_2");
int DlsymVersionV2()
{
    return EXPECT_RETURN_VALUE_2;
}

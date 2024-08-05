#include "dlopen_common.h"
#include "ldso_gtest_util.h"

int g_testNumber = TEST_NUM_1000;

int DlopenCommon()
{
    return g_testNumber;
}
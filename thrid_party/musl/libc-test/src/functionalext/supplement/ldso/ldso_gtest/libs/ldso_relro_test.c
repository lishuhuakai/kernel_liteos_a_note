#include "dlopen_common.h"
#include "ldso_gtest_util.h"

int GetRelroValue()
{
    return g_testNumber / TEST_NUM_20;
}
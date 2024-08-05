#include "ldso_gtest_util.h"

void LdsoDlsymDependentImpl() {}

extern int g_num;
int g_num = TEST_NUM_10;
int GetNum()
{
    return g_num;
}
#include "ldso_gtest_util.h"

int g_num = TEST_NUM_10;
int DlsymGetTheSymbolImpl()
{
    return g_num;
}
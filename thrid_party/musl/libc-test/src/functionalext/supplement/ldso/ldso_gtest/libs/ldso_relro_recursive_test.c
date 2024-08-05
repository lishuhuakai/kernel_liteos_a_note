#include "ldso_gtest_util.h"
#include "ldso_relro_test.h"

int GetRelroRecursiveValue()
{
    return GetRelroValue() * TEST_NUM_2;
}
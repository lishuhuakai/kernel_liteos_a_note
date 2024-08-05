#include <dlfcn.h>

#include "ldso_gtest_util.h"
#include "ldso_ns.h"
#include "ldso_ns_value.h"

static const int NS_ROOT_NUM = TEST_NUM_12;

__attribute__((weak)) int GetNsImplNum();

int GetNsRootNum()
{
    return NS_ROOT_NUM;
}

int GetNsOneNum()
{
    return NS_ONE_NUM;
}

int GetNsTwoNum()
{
    return NS_TWO_NUM;
}

int GetNsOneImplNum()
{
    if (GetNsImplNum) {
        return GetNsImplNum();
    }
    return 0;
}

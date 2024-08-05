#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "ldso_gtest_util.h"

int32_t g_testValue = TEST_NUM_10;
static bool* g_isClosed = NULL;

void DlopenNodeleteSetIsClosedPtr(bool* isClosed)
{
    g_isClosed = isClosed;
}

__attribute__((destructor)) static void CloseProcess()
{
    if (g_isClosed) {
        *g_isClosed = true;
    }
}
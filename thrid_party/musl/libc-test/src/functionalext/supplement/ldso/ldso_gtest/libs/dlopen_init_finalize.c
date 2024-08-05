#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "dlopen_init_finalize.h"
#include "ldso_gtest_util.h"

int32_t volatile g_initOrder = 0;
void (*g_finalizeCallback)(int32_t order) = NULL;

void InitProcess(int32_t order)
{
    g_initOrder = g_initOrder * TEST_NUM_10 + order;
}

void FinalizeProcess(int32_t order)
{
    if (g_finalizeCallback) {
        g_finalizeCallback(order);
    }
}

void SetFinalizeCallback(void (*callback)(int32_t order))
{
    g_finalizeCallback = callback;
}

__attribute__((constructor)) static void Init()
{
    InitProcess(PROCESS_ORDER_1);
}

__attribute__((destructor)) static void Finalize()
{
    FinalizeProcess(PROCESS_ORDER_1);
}

void DlopenInitFinalize()
{
    DlopenInitFinalize1();
    DlopenInitFinalize2();
}
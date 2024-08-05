#include <stdint.h>
#include "ldso_gtest_util.h"

__attribute__((weak)) void InitProcess(int32_t order);
__attribute__((weak)) void FinalizeProcess(int32_t order);

__attribute__((constructor)) static void Init()
{
    InitProcess(PROCESS_ORDER_3);
}

__attribute__((destructor)) static void Finalize()
{
    FinalizeProcess(PROCESS_ORDER_3);
}

void DlopenInitFinalize2() {}
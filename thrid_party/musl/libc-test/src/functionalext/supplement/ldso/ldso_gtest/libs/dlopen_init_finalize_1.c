#include <stdint.h>
#include "dlopen_init_finalize.h"
#include "ldso_gtest_util.h"

__attribute__((weak)) void InitProcess(int32_t order);
__attribute__((weak)) void FinalizeProcess(int32_t order);

__attribute__((constructor)) static void Init()
{
    InitProcess(PROCESS_ORDER_2);
}

__attribute__((destructor)) static void Finalize()
{
    FinalizeProcess(PROCESS_ORDER_2);
}

void DlopenInitFinalize1()
{
    DlopenInitFinalize2();
}
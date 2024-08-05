#include <stdint.h>
#include <stdlib.h>
#include "ldso_gtest_util.h"

static size_t g_count = 0;
static uint64_t g_typeId = 0;
static void* g_address = NULL;
static void* g_diag = NULL;
char g_buf[SIZE_1024_SQUARE];

__attribute__((aligned(OFFSET_4096))) void __cfi_check(uint64_t call_site_type_id, void *target_addr, void *diag)
{
    ++g_count;
    g_typeId = call_site_type_id;
    g_address = target_addr;
    g_diag = diag;
}

size_t GetCount()
{
    return g_count;
}

uint64_t GetTypeId()
{
    return g_typeId;
}

void* GetAddress()
{
    return g_address;
}

void* GetDiag()
{
    return g_diag;
}

void* GetGlobalAddress()
{
    return &g_count;
}

#include <stdint.h>
#include "securec.h"
#include "prt_mem.h"
#include "prt_module.h"

void *calloc(size_t nitems, size_t size)
{
    size_t real_size;
    void *ptr = NULL;

    if ((nitems == 0) || (size == 0) || (nitems > (UINT32_MAX / size))) {
        return NULL;
    }

    real_size = (size_t)(nitems * size);
    ptr = PRT_MemAlloc(OS_MID_SYS, 0, real_size);
    if (ptr != NULL) {
        (void)memset_s(ptr, real_size, 0, real_size);
    }
    return ptr;
}

void free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }

    (void)PRT_MemFree(OS_MID_SYS, ptr);
}

void *malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    return PRT_MemAlloc(OS_MID_SYS, 0, size);
}

void *zalloc(size_t size)
{
    void *ptr = NULL;

    if (size == 0) {
        return NULL;
    }

    ptr = PRT_MemAlloc(OS_MID_SYS, 0, size);
    if (ptr != NULL) {
        (void)memset_s(ptr, size, 0, size);
    }
    return ptr;
}

void *memalign(size_t boundary, size_t size)
{
    if (size == 0) {
        return NULL;
    }

    if ((boundary % sizeof(uintptr_t) != 0)) {
        return NULL;
    }

    return PRT_MemAlloc(OS_MID_SYS, 0, size);
}

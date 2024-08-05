#define _GNU_SOURCE 1
#include <sched.h>
#include <malloc.h>

cpu_set_t* __sched_cpualloc(size_t count)
{
    // The static analyzer complains that CPU_ALLOC_SIZE eventually expands to
    // N * sizeof(unsigned long), which is incompatible with cpu_set_t. This is
    // on purpose.
    return (cpu_set_t*) malloc(CPU_ALLOC_SIZE(count)); // NOLINT
}

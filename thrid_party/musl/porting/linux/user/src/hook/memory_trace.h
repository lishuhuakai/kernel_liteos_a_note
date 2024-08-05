#ifndef _MEMORY_TRACE_H
#define _MEMORY_TRACE_H

#include <stdbool.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif
void memtrace(void* addr, size_t size, const char* tag, bool is_using);
#ifdef __cplusplus
}
#endif
#endif

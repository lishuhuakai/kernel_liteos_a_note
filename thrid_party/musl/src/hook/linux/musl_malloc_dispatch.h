/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _MUSL_MALLOC_DISPATCH_H
#define _MUSL_MALLOC_DISPATCH_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef void* (*MallocMmapType) (void*, size_t, int, int, int, off_t);
typedef int (*MallocMunmapType) (void*, size_t);
typedef void* (*MallocMallocType)(size_t);
typedef void* (*MallocReallocType)(void*, size_t);
typedef void* (*MallocCallocType)(size_t, size_t);
typedef void* (*MallocVallocType)(size_t);
typedef void (*MallocFreeType)(void*);
typedef void* (*MallocAlignedAllocType)(size_t, size_t);
typedef size_t (*MallocMallocUsableSizeType)(void*);
typedef int (*MallocPrctlType)(int, ...);

typedef struct mallinfo (*MallinfoType)(void);
typedef struct mallinfo2 (*Mallinfo2Type)(void);
typedef int (*MallocIterateType)(void*, size_t, void (*callback)(void*, size_t, void*), void*);
typedef void (*MallocDisableType)(void);
typedef void (*MallocEnableType)(void);
typedef int (*MallocInfoType)(int, FILE*);
typedef void (*MallocStatsPrintType)(void (*) (void *, const char *), void *, const char *);
typedef int (*MalloptType)(int, int);
typedef ssize_t (*MallocBacktraceType)(void*, uintptr_t*, size_t);
typedef void (*MemTrace)(void*, size_t, const char*, bool);

typedef bool (*GetHookFlagType)();
typedef bool (*SetHookFlagType)(bool);
typedef bool (*SendHookMiscData)(uint64_t, const char*, size_t, uint32_t);
typedef void* (*GetHookConfig)();

struct MallocDispatchType {
	MallocMmapType mmap;
	MallocMunmapType munmap;
	MallocMallocType malloc;
	MallocCallocType calloc;
	MallocReallocType realloc;
	MallocVallocType valloc;
	MallocFreeType free;
	MallocAlignedAllocType aligned_alloc;
	MallocMallocUsableSizeType malloc_usable_size;
	MallinfoType mallinfo;
	Mallinfo2Type mallinfo2;
	MallocIterateType malloc_iterate;
	MallocDisableType malloc_disable;
	MallocEnableType malloc_enable;
	MallocInfoType malloc_info;
	MallocStatsPrintType malloc_stats_print;
	MalloptType mallopt;
	MallocBacktraceType malloc_backtrace;
	GetHookFlagType get_hook_flag;
	SetHookFlagType set_hook_flag;
	MemTrace memtrace;
	MallocPrctlType prctl;
};
#ifdef __cplusplus
}
#endif

#endif

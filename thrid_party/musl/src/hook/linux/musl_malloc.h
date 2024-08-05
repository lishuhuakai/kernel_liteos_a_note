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

#ifndef _MUSL_MALLOC_H
#define _MUSL_MALLOC_H

#include "malloc.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HOOK_ENABLE
#define MuslMalloc(func) __libc_ ## func
#else
#define MuslMalloc(func) func
#endif

#ifdef USE_JEMALLOC
extern void* je_malloc(size_t size);
extern void* je_calloc(size_t count, size_t size);
extern void* je_realloc(void* p, size_t newsize);
extern void je_free(void* p);
#define MuslFunc(func) je_ ## func
#else
extern void *__libc_malloc(size_t size);
extern void __libc_free(void* p);
extern void *hook_calloc(size_t, size_t);
#define hook_malloc __libc_malloc
#define hook_free __libc_free
#define hook_realloc __libc_realloc
#define MuslFunc(func) hook_ ## func
#endif

void *__libc_mmap(void*, size_t, int, int, int, off_t);
int __libc_munmap(void*, size_t);
void *__libc_calloc(size_t, size_t);
void *__libc_realloc(void *, size_t);
void *__libc_valloc(size_t);
void *__libc_aligned_alloc(size_t, size_t);
size_t __libc_malloc_usable_size(void *);
int __libc_prctl(int, ...);

struct mallinfo2 __libc_mallinfo2(void);

int __libc_malloc_iterate(void* base, size_t size, void (*callback)(void* base, size_t size, void* arg), void* arg);
void __libc_malloc_disable(void);
void __libc_malloc_enable(void);

int __libc_malloc_info(int options, FILE* fp);
void __libc_malloc_stats_print(void (*write_cb) (void *, const char *), void *cbopaque, const char *opts);

int __libc_mallopt(int param, int value);
ssize_t __libc_malloc_backtrace(void* pointer, uintptr_t* frames, size_t frame_count);


#ifdef __cplusplus
}
#endif

#endif

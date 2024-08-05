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

#ifdef HOOK_ENABLE
#include "musl_preinit_common.h"
#include "musl_malloc.h"
#include <stdatomic.h>
#include <malloc.h>
#include <stdlib.h>

#ifdef USE_GWP_ASAN
extern void* libc_gwp_asan_malloc(size_t bytes);
extern void libc_gwp_asan_free(void *mem);
extern size_t libc_gwp_asan_malloc_usable_size(void *mem);
extern void* libc_gwp_asan_realloc(void *ptr, size_t size);
extern void* libc_gwp_asan_calloc(size_t nmemb, size_t size);
#endif

struct musl_libc_globals __musl_libc_globals;
#ifdef USE_GWP_ASAN
struct MallocDispatchType __libc_malloc_default_dispatch = {
	.malloc = libc_gwp_asan_malloc,
	.free = libc_gwp_asan_free,
	.mmap = MuslMalloc(mmap),
	.munmap = MuslMalloc(munmap),
	.calloc = libc_gwp_asan_calloc,
	.realloc = libc_gwp_asan_realloc,
	.malloc_usable_size = libc_gwp_asan_malloc_usable_size,
	.prctl = MuslMalloc(prctl),
	.aligned_alloc = MuslMalloc(aligned_alloc),
};
#else
struct MallocDispatchType __libc_malloc_default_dispatch = {
	.malloc = MuslFunc(malloc),
	.free = MuslFunc(free),
	.mmap = MuslMalloc(mmap),
	.munmap = MuslMalloc(munmap),
	.calloc = MuslFunc(calloc),
	.realloc = MuslFunc(realloc),
	.malloc_usable_size = MuslMalloc(malloc_usable_size),
	.prctl = MuslMalloc(prctl),
	.aligned_alloc = MuslMalloc(aligned_alloc),
};
#endif

volatile atomic_bool __hook_enable_hook_flag;
volatile atomic_bool __memleak_hook_flag;

#endif

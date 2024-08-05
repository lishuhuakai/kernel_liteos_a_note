/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _GWP_ASAN_H
#define _GWP_ASAN_H
#ifdef USE_GWP_ASAN

#include <dlfcn.h>
#include <stdint.h>
#include <stddef.h>

#define GWP_ASAN_LOG_TAG "gwp_asan"
#define GWP_ASAN_LOG_DIR "/data/local/tmp/"

typedef void (*printf_t)(const char *fmt, ...);
typedef void (*printf_backtrace_t)(uintptr_t*, size_t, printf_t);
typedef size_t (*segv_backtrace_t)(uintptr_t*, size_t, void*);
typedef size_t (*backtrace_t)(size_t*, size_t);

// Note that "compiler-rt/lib/gwp_asan/gwp_asan_c_interface.cpp" in the llvm code need to be modified synchronously.
typedef struct gwp_asan_option {
    bool help;
    bool enable;
    bool install_fork_handlers;
    bool install_signal_handlers;
    int sample_rate;
    int max_simultaneous_allocations;
    backtrace_t backtrace;
    printf_t gwp_asan_printf;
    printf_backtrace_t printf_backtrace;
    segv_backtrace_t segv_backtrace;
} gwp_asan_option;

typedef struct unwind_info {
    size_t fp;
    size_t lr;
} unwind_info;

size_t libc_gwp_asan_unwind_fast(size_t *frame_buf, size_t max_record_stack,
                                 __attribute__((unused)) void *signal_context);

bool libc_gwp_asan_has_free_mem();
bool libc_gwp_asan_ptr_is_mine(void *addr);
bool may_init_gwp_asan(bool force_init);

#endif
#endif

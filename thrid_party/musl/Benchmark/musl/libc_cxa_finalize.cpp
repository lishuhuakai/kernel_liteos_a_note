/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
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
#include "util.h"

static int g_count[8] = {1, 4, 16, 32, 64, 128, 256, 1024};

extern "C" int __cxa_atexit(void (*f)(void *), void *, void *);
extern "C" void __cxa_finalize(void *);

static void call(void *p)
{
    if (p != NULL)
        ((void (*)(void))(uintptr_t)p)();
}

static void DoNothing() {}

static void Bm_function_cxa_finalize(benchmark::State &state)
{
    uintptr_t dummy;
    __cxa_atexit(call, reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(DoNothing)),
        reinterpret_cast<void*>(&dummy));

    for (auto _ : state) {
        __cxa_finalize((void*)&dummy);
    }
}

static void Bm_function_cxa_finalize_dynamic(benchmark::State &state)
{
    uintptr_t dummy;

    for (auto _ : state) {
        state.PauseTiming();
        for (int i = 0; i < g_count[state.range(0)]; i++) {
            __cxa_atexit(call, reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(DoNothing)),
                reinterpret_cast<void*>(&dummy));
        }
        state.ResumeTiming();
        __cxa_finalize((void*)&dummy);
    }
}

MUSL_BENCHMARK(Bm_function_cxa_finalize);
MUSL_BENCHMARK_WITH_ARG(Bm_function_cxa_finalize_dynamic, "BENCHMARK_8");
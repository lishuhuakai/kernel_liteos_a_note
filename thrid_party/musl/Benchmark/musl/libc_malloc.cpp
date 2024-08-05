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

#if defined __APPLE__
#include <malloc/malloc.h>
#else
#include "malloc.h"
#endif
#include "cstdlib"
#include "util.h"

using namespace std;

constexpr int MALLOC_SIZE = 2;

static const vector<int> blockSize {
    1,
    4,
    8,
    16,
    64,
    256,
    1024,
    4096
};


static void PrepareCallocArgs(benchmark::internal::Benchmark* b)
{
    for (auto l : blockSize) {
        for (auto f : blockSize) {
            b->Args({l, f});
        }
    }
}

static void Bm_function_Calloc(benchmark::State& state)
{
    int m = state.range(0);
    int n = state.range(1);
    void* ptr;
    OpenTcache();
    for (auto _ : state) {
        benchmark::DoNotOptimize(ptr = calloc(m, n));
        if (ptr) {
            free(ptr);
        }
    }
}

static void Bm_function_realloc_twice(benchmark::State &state)
{
    size_t size = MALLOC_SIZE;
    char *p =  (char *)malloc(size);
    if (p == nullptr) {
        perror("malloc Je_realloc");
    }
    OpenTcache();
    for (auto _ : state) {
        benchmark::DoNotOptimize(realloc(p, size * 2));
    }

    if (p != nullptr) {
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_realloc_half(benchmark::State &state)
{
    size_t size = MALLOC_SIZE;
    char *p =  (char *)malloc(size);
    if (p == nullptr) {
        perror("malloc Je_realloc");
    }
    OpenTcache();
    for (auto _ : state) {
        benchmark::DoNotOptimize(realloc(p, size / 2));
    }

    if (p != nullptr) {
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_realloc_equal(benchmark::State &state)
{
    size_t size = MALLOC_SIZE;
    char *p =  (char *)malloc(size);
    if (p == nullptr) {
        perror("malloc Je_realloc");
    }
    OpenTcache();
    for (auto _ : state) {
        benchmark::DoNotOptimize(realloc(p, size));
    }

    if (p != nullptr) {
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_malloc_usable_size(benchmark::State &state)
{
    char *p =  (char *)malloc(MALLOC_SIZE);
    if (p == nullptr) {
        perror("malloc Je_malloc_usable_size");
    }
    OpenTcache();
    for (auto _ : state) {
#if defined __APPLE__
        benchmark::DoNotOptimize(malloc_size(p));
#else
        benchmark::DoNotOptimize(malloc_usable_size(p));
#endif
    }

    if (p != nullptr) {
        free(p);
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_realloc_twice);
MUSL_BENCHMARK(Bm_function_realloc_half);
MUSL_BENCHMARK(Bm_function_realloc_equal);
MUSL_BENCHMARK(Bm_function_malloc_usable_size);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Calloc, PrepareCallocArgs);

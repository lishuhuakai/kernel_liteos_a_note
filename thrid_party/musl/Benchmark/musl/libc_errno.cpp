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

#include "cerrno"
#include "util.h"

using namespace std;

constexpr int BUFFERSIZE = 1024;
char g_errorBuffer[BUFFERSIZE];

static void Bm_function_Strerror_noerror(benchmark::State &state)
{
    int e = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror(e));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_enoent(benchmark::State &state)
{
    int e = 2;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror(e));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_enomem(benchmark::State &state)
{
    int e = 12;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror(e));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_eacces(benchmark::State &state)
{
    int e = 13;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror(e));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_eexist(benchmark::State &state)
{
    int e = 17;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror(e));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_einval(benchmark::State &state)
{
    int e = 22;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror(e));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_erofs(benchmark::State &state)
{
    int e = 30;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror(e));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_etimeout(benchmark::State &state)
{
    int e = 110;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror(e));
    }
    state.SetItemsProcessed(state.iterations());
}

// Used to convert the error number into the corresponding error message
static void Bm_function_Strerror_r_noerror(benchmark::State &state)
{
    int e = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror_r(e, g_errorBuffer, sizeof(g_errorBuffer)));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_r_enoent(benchmark::State &state)
{
    int e = 2;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror_r(e, g_errorBuffer, sizeof(g_errorBuffer)));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_r_enomem(benchmark::State &state)
{
    int e = 12;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror_r(e, g_errorBuffer, sizeof(g_errorBuffer)));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_r_eacces(benchmark::State &state)
{
    int e = 13;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror_r(e, g_errorBuffer, sizeof(g_errorBuffer)));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_r_eexist(benchmark::State &state)
{
    int e = 17;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror_r(e, g_errorBuffer, sizeof(g_errorBuffer)));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_r_einval(benchmark::State &state)
{
    int e = 22;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror_r(e, g_errorBuffer, sizeof(g_errorBuffer)));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_r_erofs(benchmark::State &state)
{
    int e = 30;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror_r(e, g_errorBuffer, sizeof(g_errorBuffer)));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Strerror_r_etimeout(benchmark::State &state)
{
    int e = 110;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strerror_r(e, g_errorBuffer, sizeof(g_errorBuffer)));
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Strerror_noerror);
MUSL_BENCHMARK(Bm_function_Strerror_enoent);
MUSL_BENCHMARK(Bm_function_Strerror_enomem);
MUSL_BENCHMARK(Bm_function_Strerror_eacces);
MUSL_BENCHMARK(Bm_function_Strerror_eexist);
MUSL_BENCHMARK(Bm_function_Strerror_einval);
MUSL_BENCHMARK(Bm_function_Strerror_erofs);
MUSL_BENCHMARK(Bm_function_Strerror_etimeout);
MUSL_BENCHMARK(Bm_function_Strerror_r_noerror);
MUSL_BENCHMARK(Bm_function_Strerror_r_enoent);
MUSL_BENCHMARK(Bm_function_Strerror_r_enomem);
MUSL_BENCHMARK(Bm_function_Strerror_r_eacces);
MUSL_BENCHMARK(Bm_function_Strerror_r_eexist);
MUSL_BENCHMARK(Bm_function_Strerror_r_einval);
MUSL_BENCHMARK(Bm_function_Strerror_r_erofs);
MUSL_BENCHMARK(Bm_function_Strerror_r_etimeout);
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

#include <clocale>
#include <langinfo.h>
#include "sys/types.h"
#if not defined __APPLE__
#include "sys/epoll.h"
#endif
#include "sys/stat.h"
#include "fcntl.h"
#include "cstdarg"
#include "cwchar"
#include "ctime"
#include "cmath"
#include "cstring"
#include "cstdlib"
#include "pthread.h"
#include "cstdio"
#include "unistd.h"
#include "util.h"
#include "sys/syscall.h"
#if defined __APPLE__
#include <xlocale.h>
#endif

using namespace std;

static void Bm_function_Setlocale_All(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_ALL, nullptr));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_All1(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_ALL, "C"));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_All2(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_ALL, ""));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_Collate(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_COLLATE, nullptr));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_Ctype(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_CTYPE, nullptr));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Setlocale_Time(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(setlocale(LC_TIME, nullptr));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Locale_nl_langinfo(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(nl_langinfo(CODESET));
    }
}

static void Bm_function_Locale_localeconv(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(localeconv());
    }
}

static void Bm_function_Uselocale(benchmark::State &state)
{
    locale_t oldLocale;
    locale_t newLocale = newlocale(LC_ALL_MASK, "zh_CN.UTF-8", nullptr);
    for (auto _ : state) {
        oldLocale = uselocale(newLocale);
        newLocale = uselocale(oldLocale);
    }
    state.SetBytesProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Setlocale_All);
MUSL_BENCHMARK(Bm_function_Setlocale_All1);
MUSL_BENCHMARK(Bm_function_Setlocale_All2);
MUSL_BENCHMARK(Bm_function_Setlocale_Collate);
MUSL_BENCHMARK(Bm_function_Setlocale_Ctype);
MUSL_BENCHMARK(Bm_function_Setlocale_Time);
MUSL_BENCHMARK(Bm_function_Locale_nl_langinfo);
MUSL_BENCHMARK(Bm_function_Locale_localeconv);
MUSL_BENCHMARK(Bm_function_Uselocale);

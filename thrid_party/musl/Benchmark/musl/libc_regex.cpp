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

#include <regex.h>
#include <vector>
#include "util.h"

static int g_count[8] = {1, 2, 3, 4, 5, 6, 7, 8};

static const std::vector<int> regCompFlags {
    0,
    REG_EXTENDED,
    REG_ICASE,
    REG_NOSUB,
    REG_NEWLINE,
};

static void PrepareRegcompArgs(benchmark::internal::Benchmark* b)
{
    for (auto l : regCompFlags) {
        b->Args({l});
    }
}

static void Bm_function_Regcomp(benchmark::State &state)
{
    const char* pattern = "hello.*world";
    int flag = state.range(0);
    OpenTcache();
    for (auto _state: state) {
        regex_t reg;
        benchmark::DoNotOptimize(regcomp(&reg, pattern, flag));
        regfree(&reg);
    }
}

static void Bm_function_Regexec(benchmark::State &state)
{
    const char* pattern = "hello.*world";
    int flag = state.range(0);
    regex_t reg;
    if (regcomp(&reg, pattern, flag)) {
        perror("regcomp");
        exit(-1);
    }
    OpenTcache();
    regmatch_t pmatch;
    for (auto _state: state) {
        benchmark::DoNotOptimize(regexec(&reg, "hello test world", 1, &pmatch, 0));
    }

    regfree(&reg);
}

static void Bm_function_Regall(benchmark::State &state)
{
    const char* pattern = "hello.*world";
    int flag = state.range(0);
    regex_t reg;

    OpenTcache();
    regmatch_t pmatch;
    for (auto _ : state) {
        if (regcomp(&reg, pattern, flag)) {
            perror("regcomp");
        }
        for (int i = 0; i < g_count[state.range(0)]; i++) {
            regexec(&reg, "hello test world", 1, &pmatch, 0);
        }
        regfree(&reg);
    }
}

MUSL_BENCHMARK_WITH_APPLY(Bm_function_Regcomp, PrepareRegcompArgs);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Regexec, PrepareRegcompArgs);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Regall, "BENCHMARK_8");

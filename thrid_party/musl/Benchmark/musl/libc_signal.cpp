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

#include "csignal"
#include "util.h"

static void SignalHandler(int i) {}

static void Bm_function_Sigaction(benchmark::State &state)
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = SignalHandler;

    for (auto _ : state) {
        benchmark::DoNotOptimize(sigaction(SIGUSR1, &sa, nullptr));
    }
}

static void Bm_function_Sigemptyset(benchmark::State &state)
{
    for (auto _ : state) {
        sigset_t set;
        benchmark::DoNotOptimize(sigemptyset(&set));
    }
}

static void Bm_function_Sigaltstack(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        stack_t ss, old_ss;
        ss.ss_sp = malloc(SIGSTKSZ);
        if (ss.ss_sp == nullptr) {
            perror("malloc");
        }

        ss.ss_size = SIGSTKSZ;
        ss.ss_flags = 0;
        state.ResumeTiming();
        if (sigaltstack(&ss, &old_ss) == -1) {
            perror("sigaltstack");
        }

        state.PauseTiming();
        if (sigaltstack(&old_ss, nullptr) == -1) {
            perror("sigaltstack");
        }
        free(ss.ss_sp);
        state.ResumeTiming();
    }
}

static void Bm_function_Sigdelset(benchmark::State &state)
{
    sigset_t set;
    for (auto _ : state) {
        sigaddset(&set, SIGINT);
        sigdelset(&set, SIGINT);
    }
}

static void Bm_function_Sigtimedwait(benchmark::State &state)
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGTERM);
    siginfo_t info{0};
    timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(sigtimedwait(&set, &info, &ts));
    }
}

MUSL_BENCHMARK(Bm_function_Sigaction);
MUSL_BENCHMARK(Bm_function_Sigemptyset);
MUSL_BENCHMARK(Bm_function_Sigaltstack);
MUSL_BENCHMARK(Bm_function_Sigdelset);
MUSL_BENCHMARK(Bm_function_Sigtimedwait);

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

#include "sched.h"
#include "cstdlib"
#include "unistd.h"
#include "util.h"

constexpr long MALLOC_SIZE = (1024 * 8192);

static void Bm_function_sched_yield(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(sched_yield());
    }
}

int ThreadWaitFunc(void* arg)
{
    return 0;
}

// Used to create a new process
static void Bm_function_Clone(benchmark::State &state)
{
    void *stack = malloc(MALLOC_SIZE);
    if (stack == nullptr) {
        perror("malloc clone");
    }
    int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS;
    int pid = -1;
    for (auto _ : state) {
        pid = clone(ThreadWaitFunc, (char*)stack + MALLOC_SIZE, flags, nullptr);
        if (pid == -1) {
            perror("clone proc");
        }
        sleep(1);
    }
    free(stack);
}

MUSL_BENCHMARK(Bm_function_sched_yield);
MUSL_BENCHMARK(Bm_function_Clone);
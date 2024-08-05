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

#include "sys/wait.h"
#include "sys/types.h"
#include "unistd.h"
#include "util.h"

// Wait for the child thread to finish executing
static void Bm_function_Wait(benchmark::State &state)
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork execlp ls");
    }
    for (auto _ : state) {
        if (pid == 0) {
            continue;
        } else {
            wait(nullptr);
        }
    }
}

MUSL_BENCHMARK(Bm_function_Wait);
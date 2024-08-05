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

#include "sys/select.h"
#include "sys/time.h"
#include "sys/types.h"
#include "unistd.h"
#include "util.h"

using namespace std;

constexpr int MAX_MONITOR_FDS = 2;

static void Bm_function_Select(benchmark::State &state)
{
    fd_set readfds;
    fd_set writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(0, &readfds);
    FD_SET(1, &writefds);
    for (auto _ : state) {
        benchmark::DoNotOptimize(select(MAX_MONITOR_FDS, &readfds, &writefds, 0, 0));
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Select);
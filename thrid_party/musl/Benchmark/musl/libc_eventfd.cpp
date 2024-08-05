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

#include "sys/eventfd.h"
#include "unistd.h"
#include "util.h"

// Write a value to eventfd
static void Bm_function_eventfd_write(benchmark::State &state)
{
    unsigned int initValue = 2;
    int fd = eventfd(initValue, O_NONBLOCK);
    if (fd == -1) {
        perror("evevtfd eventfd_write");
    }
    for (auto _ : state) {
        if (eventfd_write(fd, initValue + 1) != 0) {
            perror("eventfd_write proc");
        }
    }
    close(fd);
}

MUSL_BENCHMARK(Bm_function_eventfd_write);
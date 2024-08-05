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

#include "pwd.h"
#include "sys/types.h"
#include "unistd.h"
#include "util.h"

constexpr int BUF_SIZE = 16384;

static size_t AllocGetpwrBuf(char **buf)
{
    size_t bufSize = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufSize == static_cast<size_t>(-1)) {
        bufSize = BUF_SIZE;
    }

    *buf = (char *)malloc(bufSize);
    return bufSize;
}

static void Bm_function_Getpwnam_r(benchmark::State &state)
{
    struct passwd pwd;
    struct passwd *result;
    char *buf;
    size_t bufSize = AllocGetpwrBuf(&buf);
    if (buf == nullptr) {
        perror("malloc");
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(getpwnam_r("root", &pwd, buf, bufSize, &result));
    }

    free(buf);
}

static void Bm_function_Getpwuid_r(benchmark::State &state)
{
    struct passwd pwd;
    struct passwd *result;
    char *buf;
    size_t bufSize = AllocGetpwrBuf(&buf);
    if (buf == nullptr) {
        perror("malloc");
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(getpwuid_r(0, &pwd, buf, bufSize, &result));
    }

    free(buf);
}

// Obtain the user name and user information based on the user ID
static void Bm_function_Getpwuid(benchmark::State &state)
{
    uid_t uid = getuid();
    for (auto _ : state) {
        if (getpwuid(uid) == nullptr) {
            perror("getpwuid proc");
        }
    }
}

MUSL_BENCHMARK(Bm_function_Getpwnam_r);
MUSL_BENCHMARK(Bm_function_Getpwuid_r);
MUSL_BENCHMARK(Bm_function_Getpwuid);
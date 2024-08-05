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

#include "sys/stat.h"
#include "sys/types.h"
#include "fcntl.h"
#include "cstdio"
#include "cstdlib"
#include "unistd.h"
#include "util.h"

using namespace std;

static void Bm_function_Fstatat_relativepath(benchmark::State &state)
{
    struct stat st;
    int fd = -1;
    for (auto _ : state) {
        fd = fstatat(AT_FDCWD, "/dev/zero", &st, 0);
        if (fd == -1) {
            perror("fstatat relativepath");
        }
        benchmark::DoNotOptimize(fd);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fstatat_symbollink(benchmark::State &state)
{
    symlink("/etc/passwd", DATA_ROOT"/data/local/tmp/passwd_link");
    struct stat st;
    int fd = -1;
    for (auto _ : state) {
        fd = fstatat(AT_FDCWD, DATA_ROOT"/data/local/tmp/passwd_link", &st, AT_SYMLINK_NOFOLLOW);
        if (fd == -1) {
            perror("fstatat symbollink");
        }
        benchmark::DoNotOptimize(fd);
    }
    remove(DATA_ROOT"/data/local/tmp/passwd_link");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fstatat_absolutepath(benchmark::State &state)
{
    struct stat st;
    int ret = -1;
    for (auto _ : state) {
        ret = fstatat(0, "/dev/zero", &st, 0);
        if (ret == -1) {
            perror("fstatat absolutepath");
        }
        benchmark::DoNotOptimize(ret);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fstat64(benchmark::State &state)
{
    struct stat buf;
    int fd = open("/etc/passwd", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open fstat64");
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(fstat(fd, &buf));
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Mkdir(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(mkdir(DATA_ROOT"/data/data/test_mkdir",
                                S_IRWXU | S_IRWXG | S_IXGRP | S_IROTH | S_IXOTH));
        state.PauseTiming();
        rmdir(DATA_ROOT"/data/data/test_mkdir");
        state.ResumeTiming();
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Mkdirat(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(mkdirat(AT_FDCWD, DATA_ROOT"/data/data/test_mkdirat",
                                S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH));
        state.PauseTiming();
        rmdir(DATA_ROOT"/data/data/test_mkdirat");
        state.ResumeTiming();
    }
}

MUSL_BENCHMARK(Bm_function_Fstatat_relativepath);
MUSL_BENCHMARK(Bm_function_Fstatat_symbollink);
MUSL_BENCHMARK(Bm_function_Fstatat_absolutepath);
MUSL_BENCHMARK(Bm_function_Fstat64);
MUSL_BENCHMARK(Bm_function_Mkdir);
MUSL_BENCHMARK(Bm_function_Mkdirat);

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
#include "dirent.h"
#include "unistd.h"
#include "fcntl.h"
#include "util.h"

using namespace std;

static void Bm_function_Opendir(benchmark::State &state)
{
    for (auto _ : state) {
        DIR *dir = opendir("/dev/block");
        if (dir == nullptr) {
            perror("opendir proc");
        }
        benchmark::DoNotOptimize(dir);
        state.PauseTiming();
        closedir(dir);
        state.ResumeTiming();
    }
}

static void Bm_function_ScanDir(benchmark::State &state)
{
    const char* dir = "/dev/block";
    struct dirent **entry = nullptr;
    for (auto _ : state) {
        int n = scandir(dir, &entry, nullptr, alphasort);
        if (n <= 0) {
            continue;
        }
        state.PauseTiming();
        for (int i = 0; i < n; i++) {
            free(entry[i]);
        }
        free(entry);
        state.ResumeTiming();
    }
}

static void Bm_function_Closedir(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        DIR *dir = opendir("/dev/block");
        if (dir == nullptr) {
            perror("opendir proc");
        }
        state.ResumeTiming();

        benchmark::DoNotOptimize(closedir(dir));
    }
}

static void Bm_function_Readdir(benchmark::State &state)
{
    DIR *dir = opendir("/dev/block");
    if (dir == nullptr) {
        perror("opendir proc");
    }

    for (auto _ : state) {
        struct dirent *entry = nullptr;
        do {
            entry = readdir(dir);
            benchmark::DoNotOptimize(entry);
        } while (entry != nullptr);
    }
    closedir(dir);
}

// Open the table of contents
static void Bm_function_Fdopendir(benchmark::State &state)
{
    int fd = open("/dev/", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open fdopendir");
    }
    for (auto _ : state) {
        DIR *dir = fdopendir(fd);
        if (dir == nullptr) {
            perror("fdopendir proc");
        }
        benchmark::DoNotOptimize(dir);
    }
    close(fd);
}

static void Bm_function_Rewinddir(benchmark::State &state)
{
    DIR *dir = opendir("/data/local");
    if (dir == nullptr) {
        perror("opendir rewinddir");
    }
    while (readdir(dir) != nullptr) {}
    for (auto _ : state) {
        rewinddir(dir);
    }
    closedir(dir);
}

static void Bm_function_Open_Dir(benchmark::State &state)
{
    const char *filename = "/dev/block";
    for (auto _ : state) {
        int fd = open(filename, O_RDONLY|O_DIRECTORY|O_CLOEXEC);
        if (fd == -1) {
            perror("open_dir proc");
        }
        benchmark::DoNotOptimize(fd);
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Opendir);
MUSL_BENCHMARK(Bm_function_ScanDir);
MUSL_BENCHMARK(Bm_function_Closedir);
MUSL_BENCHMARK(Bm_function_Readdir);
MUSL_BENCHMARK(Bm_function_Fdopendir);
MUSL_BENCHMARK(Bm_function_Rewinddir);
MUSL_BENCHMARK(Bm_function_Open_Dir);

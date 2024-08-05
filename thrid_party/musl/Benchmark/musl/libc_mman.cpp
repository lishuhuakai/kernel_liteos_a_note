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

#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "unistd.h"
#include "cstdio"
#if defined __APPLE__
#include <malloc/malloc.h>
#else
#include "malloc.h"
#endif
#include "util.h"

using namespace std;

static const vector<int> mmapFlags = {
    MAP_PRIVATE | MAP_ANONYMOUS,
#if not defined __APPLE__
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_2MB,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_1GB,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB | MAP_POPULATE,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_2MB | MAP_POPULATE,
    MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGE_1GB | MAP_POPULATE,
#endif
};

static const vector<int> msyncFlags = {
    MS_ASYNC,
    MS_SYNC,
    MS_INVALIDATE,
};

static const vector<int> mmapLength {
    8,
    16,
    64,
    1 * K,
    4 * K,
    64 * K,
    256 * K,
    1 * M,
    4 * M,
    64 * M,
    256 * M,
    1 * G,
};

static const vector<int> mdviseType {
    MADV_NORMAL,
    MADV_RANDOM,
    MADV_SEQUENTIAL,
    MADV_WILLNEED,
    MADV_DONTNEED,
    MADV_FREE,
#if not defined __APPLE__
    MADV_REMOVE,
    MADV_HUGEPAGE,
    MADV_SOFT_OFFLINE
#endif
};

static const vector<int> mprotectLength {
    1,
    2,
    4,
    32,
    1 * K,
    4 * K,
    32 * K,
    1 * M,
};

static void PrepareArgs(benchmark::internal::Benchmark* b)
{
    for (auto l : mmapLength) {
        for (auto f : mmapFlags) {
            b->Args({l, f});
        }
    }
}

static void PrepareArgsInMdvise(benchmark::internal::Benchmark* b)
{
    for (auto l : mmapLength) {
        for (auto t : mdviseType) {
            b->Args({l, t});
        }
    }
}

static void PrepareArgsInMremap(benchmark::internal::Benchmark* b)
{
    for (auto oldLength : mmapLength) {
        for (auto newLength : mmapLength) {
            b->Args({oldLength, newLength});
        }
    }
}

static void PrepareArgsInMsync(benchmark::internal::Benchmark* b)
{
    for (auto l : mmapLength) {
        for (auto f : msyncFlags) {
            b->Args({l, f});
        }
    }
}

static void Bm_function_Mmap_anonymous(benchmark::State &state)
{
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, flags, -1, 0);
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            state.PauseTiming();
            munmap(mem, length);
            state.ResumeTiming();
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Munmap_anonymous(benchmark::State &state)
{
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        state.PauseTiming();
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, flags, -1, 0);
        state.ResumeTiming();
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            munmap(mem, length);
        }
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Mmap_fd(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open /dev/zero failed.");
    }
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, flags, fd, 0);
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            state.PauseTiming();
            munmap(mem, length);
            state.ResumeTiming();
        }
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Munmap_fd(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open /dev/zero failed.");
    }
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        state.PauseTiming();
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, flags, fd, 0);
        state.ResumeTiming();
        if (mem != MAP_FAILED) {
            benchmark::DoNotOptimize(mem);
            munmap(mem, length);
        }
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Madvise(benchmark::State &state)
{
    size_t length = state.range(0);
    int type = state.range(1);
    int *addr = (int*)mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(madvise(addr, length, type));
    }
    madvise(addr, length, MADV_NORMAL);
    munmap(addr, length);
}

static void Bm_function_Mremap(benchmark::State &state)
{
    size_t oldLength = state.range(0);
    size_t newLength = state.range(1);

    for (auto _ : state) {
        state.PauseTiming();
        void *oldAddr = mmap(nullptr, oldLength, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        state.ResumeTiming();

        if (oldAddr != MAP_FAILED) {
#if defined __APPLE__
            void *newAddr = MAP_FAILED;
#else
            void *newAddr = mremap(oldAddr, oldLength, newLength, MREMAP_MAYMOVE);
#endif
            if (newAddr != MAP_FAILED) {
                state.PauseTiming();
                munmap(newAddr, newLength);
                state.ResumeTiming();
            } else {
                perror("mmap");
            }
        }
    }
}

static void Bm_function_Msync(benchmark::State &state)
{
    size_t length = state.range(0);
    int flags = state.range(1);
    for (auto _ : state) {
        state.PauseTiming();
        char* mem = (char *)mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        state.ResumeTiming();

        if (mem != MAP_FAILED) {
            msync(mem, length, flags);

            state.PauseTiming();
            munmap(mem, length);
            state.ResumeTiming();
        }
    }
}

static void Bm_function_mprotect(benchmark::State &state)
{
    size_t pagesize = sysconf(_SC_PAGE_SIZE);
    size_t size = pagesize * mprotectLength[state.range(0)];
#if defined __APPLE__
    void *pages;
    posix_memalign(&pages, pagesize, size);
#else
    void *pages = memalign(pagesize, size);
#endif

    for (auto _ : state) {
        benchmark::DoNotOptimize(mprotect(pages, size, PROT_READ | PROT_WRITE));
    }
    state.SetItemsProcessed(state.iterations());

    free(pages);
}

// Used to unlock some or all of a process's virtual memory
// allowing it to be swapped out to swap space on disk
static void Bm_function_Mlock_Munlock(benchmark::State &state)
{
    size_t length = state.range(0);
    void *addr = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (addr == MAP_FAILED) {
        perror("mmap munlock");
    }

    for (auto _ : state) {
        if (mlock(addr, length) != 0) {
            perror("mlock munlock");
        }

        if (munlock(addr, length) != 0) {
            perror("munlock proc");
        }
    }
    munmap(addr, length);
}

MUSL_BENCHMARK_WITH_APPLY(Bm_function_Mmap_anonymous, PrepareArgs);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Munmap_anonymous, PrepareArgs);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Mmap_fd, PrepareArgs);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Munmap_fd, PrepareArgs);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Madvise, PrepareArgsInMdvise);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Mremap, PrepareArgsInMremap);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Msync, PrepareArgsInMsync);
MUSL_BENCHMARK_WITH_ARG(Bm_function_mprotect, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Mlock_Munlock, "COMMON_ARGS");

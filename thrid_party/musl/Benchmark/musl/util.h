/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#pragma once

#include <benchmark/benchmark.h>
#include <stdint.h>

#include <map>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#ifdef SYSTEM_32_BIT
#define LIBACE_PATH "system/lib/platformsdk/libace.z.so"
#define LIBGLES_PATH "/system/lib/libGLESv3.so"
#else
#define LIBACE_PATH "system/lib64/platformsdk/libace.z.so"
#define LIBGLES_PATH "/vendor/lib64/chipsetsdk/libGLESv3.so"
#define LIBGLES_MALI_PATH "/vendor/lib64/chipsetsdk/libGLES_mali.so"
#endif

#if defined __APPLE__
#define DATA_ROOT "."
#else
#define DATA_ROOT 
#endif

typedef void (*BenchmarkFunc) (void);
typedef void (*ApplyBenchmarkFunc) (benchmark::internal::Benchmark*);

extern std::mutex g_benchmarkLock;

extern std::map<std::string, std::pair<BenchmarkFunc, std::string>> g_allBenchmarks;

extern std::map<std::string, std::pair<BenchmarkFunc, ApplyBenchmarkFunc>> g_applyBenchmarks;

static int __attribute__((unused)) AddApplyBenchmark(const std::string& funcName, BenchmarkFunc funcPtr,
                                                     ApplyBenchmarkFunc applyFuncPtr)
{
    g_benchmarkLock.lock();
    g_applyBenchmarks.emplace(std::string(funcName), std::make_pair(funcPtr, applyFuncPtr));
    g_benchmarkLock.unlock();
    return 0;
}

static int  __attribute__((unused)) AddBenchmark(const std::string& funcName,
                                                 BenchmarkFunc funcPtr, const std::string& arg = "")
{
    g_benchmarkLock.lock();
    g_allBenchmarks.emplace(std::string(funcName), std::make_pair(funcPtr, arg));
    g_benchmarkLock.unlock();
    return 0;
}

#define MUSL_BENCHMARK(n) \
    int _musl_benchmark_##n __attribute__((unused)) = AddBenchmark(std::string(#n), \
                                                                   reinterpret_cast<BenchmarkFunc>(n))
#define MUSL_BENCHMARK_WITH_ARG(n, arg) \
    int _musl_benchmark_##n __attribute__((unused)) = AddBenchmark(std::string(#n), \
                                                                   reinterpret_cast<BenchmarkFunc>(n), arg)

#define MUSL_BENCHMARK_WITH_APPLY(n, arg) \
    int _apply_musl_benchmark_##n __attribute__((unused)) = AddApplyBenchmark(std::string(#n), \
                                                                   reinterpret_cast<BenchmarkFunc>(n), \
                                                                   reinterpret_cast<ApplyBenchmarkFunc>(arg))

constexpr auto K = 1024;
constexpr auto M = 1024 * 1024;
constexpr auto G = 1024 * 1024 * 1024;

typedef struct bench_opts_t
{
    int cpuNum = -1;
    long iterNum = 0;
    std::string jsonPath;
} BENCH_OPTS_T;

char* AlignUpMemoy(char* origPtr, size_t alignment);

char* GetAlignedPtr(std::vector<char>* buf, size_t alignment, size_t nbytes);

wchar_t* GetAlignedPtr(std::vector<wchar_t>* buf, size_t alignment, size_t nbytes);

char* GetAlignedPtrFilled(std::vector<char>* buf, size_t alignment, size_t nbytes, char fillByte);

wchar_t* GetAlignedPtrFilled(std::vector<wchar_t>* buf, size_t alignment, size_t nbytes, wchar_t fillByte);

char* ReadJsonFile(const char *fileName);
enum JSONError {
    JSON_SUCCESS = 0,
    JOSN_ERROR_FILE_READ_FAILED,
    JOSN_ERROR_JSON_FORMAT,
};

void OpenTcache();

#define OPEN_MODE 0666

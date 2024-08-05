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

#include "cstdlib"
#include "cstdio"
#include "cstring"
#include "fcntl.h"
#include "unistd.h"
#include "cwchar"
#include "clocale"
#include "util.h"

using namespace std;

using Nier = struct {
    char name[20];
    int age;
};

static const vector<int> memalignLength {
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

static const vector<int> memalignAlign {
    8,
    16,
    64,
    256,
    1 * K,
    4 * K,
    64 * K,
    256 * K,
    1 * M,
};

static void PrepareArgsMemalign(benchmark::internal::Benchmark* b)
{
    for (auto a : memalignAlign) {
        for (auto l : memalignLength) {
            if (l >= a && l % a == 0) {
                b->Args({a, l});
            }
        }
    }
}

int CompareInt(const void *a, const void *b)
{
    int c = *(int *)a;
    int d = *(int *)b;
    if (c < d) {
        return -1;
    } else if (c > d) {
        return 1;
    } else {
        return 0;
    }
}

int CompareDouble(const void *a, const void *b)
{
    double c = *(double *)a;
    double d = *(double *)b;
    if (c == d) {
        return 0;
    } else if (c > d) {
        return 1;
    } else {
        return -1;
    }
}

int CompareString(const void *a, const void *b)
{
    const char* c = (char *)a;
    const char* d = (char *)b;
    return strcmp(c, d);
}

int CompareStruct(const void *a, const void *b)
{
    return strcmp(((Nier *)a)->name, ((Nier *)b)->name);
}

void InitRandomArray(int *arr, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        arr[i] = rand() % (2 * n);
    }
}

// Convert the string pointed to by str to floating point
static void Bm_function_Strtod(benchmark::State &state)
{
    const char *var[] = { "+2.86500000e+01", "3.1415", "29",
                          "-123.456", "1.23e5", "0x1.2p3",
                          "-inf", "123foo" };
    const char *str = var[state.range(0)];
    char *ptr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strtod(str, &ptr));
    }
}

static void Bm_function_Strtof(benchmark::State &state)
{
     const char *var[] = {"+2.86500000e+01", "3.1415", "29",
                          "-123.456", "1.23e5", "0x1.2p3",
                          "-inf", "123foo"};
    const char *str = var[state.range(0)];
    char *ptr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strtof(str, &ptr));
    }
}

static void Bm_function_Strtold(benchmark::State &state)
{
     const char *var[] = {"+2.86500000e+01", "3.1415", "29",
                          "-123.456", "1.23e5", "0x1.2p3",
                          "-inf", "123foo"};
    const char *str = var[state.range(0)];
    char *ptr;
    for (auto _ : state) {
        benchmark::DoNotOptimize(strtold(str, &ptr));
    }
}

// Used to sort elements in an array
// int type
static void Bm_function_Qsortint(benchmark::State &state)
{
    for (auto _ : state) {
        int arr[] = { 12, 89, 5, 3, 7, 1, 9, 2, 6 };
        int n = sizeof(arr) / sizeof(arr[0]);
        qsort(arr, n, sizeof(int), CompareInt);
    }
}

// double type
static void Bm_function_Qsortdouble(benchmark::State &state)
{
    for (auto _ : state) {
        double arr[] = { 34.541, 5.32, 3.56, 7.897, 1.2324, 9.34543, 5.324, 98.543, 34.665 };
        int n = sizeof(arr) / sizeof(arr[0]);
        qsort(arr, n, sizeof(double), CompareDouble);
    }
}

// string type
static void Bm_function_Qsortstring(benchmark::State &state)
{
    for (auto _ : state) {
        const char *arr[] = { "nihuangela", "xiaozhenhuniang", "our story",
                              "a language", "love", "qluhanagala",
                              "for elun", "sakuruwuma", "benchmark_musl" };
        int n = sizeof(arr) / sizeof(arr[0]);
        qsort(arr, n, sizeof(char *), CompareString);
    }
}

// struct type
static void Bm_function_Qsortstruct(benchmark::State &state)
{
    const int len = 9;
    for (auto _ : state) {
        Nier nidate[len] = { {"Meihuagao", 23}, {"Sdifenzhou", 68}, {"Amusterlang", 99},
                             {"elun", 56}, {"yishinuala", 120}, {"huajiahaochi", 22},
                             {"lunala", 66}, {"cocolou", 77}, {"xinnoqikala", 55} };
        qsort(nidate, len, sizeof(Nier), CompareStruct);
    }
}

static void Bm_function_Qsort_random(benchmark::State &state)
{
    srand(1);
    int n = state.range(0);
    int *arr1 = new int[n];
    int *arr2 = new int[n];
    InitRandomArray(arr1, n);

    for (auto _ : state)
    {
        state.PauseTiming();
        memcpy(arr2, arr1, n);
        state.ResumeTiming();
        qsort(arr2, n, sizeof(int), CompareInt);
    }

    delete[] arr1;
    delete[] arr2;
}

static void Bm_function_Getenv_TZ(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("TZ"));
    }
}

static void Bm_function_Getenv_LD_LIBRARY_PATH(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("LD_LIBRARY_PATH"));
    }
}

static void Bm_function_Getenv_LD_PRELOAD(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("LD_PRELOAD"));
    }
}

static void Bm_function_Getenv_LC_ALL(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("LC_ALL"));
    }
}

static void Bm_function_Getenv_LOGNAME(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(getenv("LOGNAME"));
    }
}

// Converts any relative path to an absolute path
static void Bm_function_Realpath(benchmark::State &state)
{
    const int bufferLen = 4096;
    const char *realpathvariable[] = { "./log", "../dev", "log/hilog", "../dev/zero", "/dev" };
    const char *path = realpathvariable[state.range(0)];
    char resolvedPath[bufferLen];
    for (auto _ : state) {
        benchmark::DoNotOptimize(realpath(path, resolvedPath));
    }
}

static void Bm_function_Posix_Memalign(benchmark::State &state)
{
    size_t align = state.range(0);
    size_t length = state.range(1);
    for (auto _ : state) {
        void *buf = nullptr;
        int ret = posix_memalign(&buf, align, length);
        if (ret) {
            perror("posix_memalign failed");
        }

        state.PauseTiming();
        free(buf);
        state.ResumeTiming();
    }
}

static void Bm_function_Mkostemps(benchmark::State &state)
{
    char fileTemplate[] = "/tmp/mkostemps-XXXXXX-test";
    for (auto _ : state) {
        int fd = mkostemps(fileTemplate, 5, O_SYNC);
        if (fd >= 0) {
            unlink(fileTemplate);
            close(fd);
        }
    }
}

// Generate a random number seed and receive a sequence of random numbers
static void Bm_function_Srand48_Lrand48(benchmark::State &state)
{
    for (auto _ : state) {
        srand48(time(nullptr));
        benchmark::DoNotOptimize(lrand48());
    }
}

// Customize the value of the environment variable and delete it
static void Bm_function_Putenv_Unsetenv(benchmark::State &state)
{
    const char *putpath[] = { "TEST_A=/usr/local/myapp", "TEST_B=/bin/yes", "TEST_C=/usr/local/bin:/usr/bin:/bin",
                              "TEST_D=vim", "TEST_E=hello", "TEST_F=en_US.UTF-8", "TEST_G=myname",
                              "TEST_H=nichenzhan" };
    const char *unsetpath[] = { "TEST_A", "TEST_B", "TEST_C", "TEST_D",
                                "TEST_E", "TEST_F", "TEST_G", "TEST_H" };
    const char *a = putpath[state.range(0)];
    const char *b = unsetpath[state.range(0)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(putenv((char*)a));
        benchmark::DoNotOptimize(unsetenv(b));
    }
}

static void Bm_function_Wcstombs(benchmark::State &state)
{
    setlocale(LC_ALL, "");
    const wchar_t wstr[] = L"z\u00df\u6c34\U0001f34c";
    size_t len = wcslen(wstr);
    char *str = (char*)malloc(sizeof(char) * (len + 1));
    for (auto _ : state) {
        benchmark::DoNotOptimize(wcstombs(str, wstr, len));
    }
    free(str);
}

void ExitFunc(){};
static void Bm_function_cxa_atexit(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(atexit(ExitFunc));
    }
}

static void Bm_function_Srandom(benchmark::State &state)
{
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    for (auto _ : state) {
        srandom(seed);
    }
}

MUSL_BENCHMARK_WITH_ARG(Bm_function_Strtod, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strtof, "BENCHMARK_8");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Strtold, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Qsortint);
MUSL_BENCHMARK(Bm_function_Qsortdouble);
MUSL_BENCHMARK(Bm_function_Qsortstring);
MUSL_BENCHMARK(Bm_function_Qsortstruct);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Qsort_random, "COMMON_ARGS");
MUSL_BENCHMARK_WITH_ARG(Bm_function_Realpath, "BENCHMARK_5");
MUSL_BENCHMARK(Bm_function_Getenv_TZ);
MUSL_BENCHMARK(Bm_function_Getenv_LD_LIBRARY_PATH);
MUSL_BENCHMARK(Bm_function_Getenv_LD_PRELOAD);
MUSL_BENCHMARK(Bm_function_Getenv_LC_ALL);
MUSL_BENCHMARK(Bm_function_Getenv_LOGNAME);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Posix_Memalign, PrepareArgsMemalign);
MUSL_BENCHMARK(Bm_function_Mkostemps);
MUSL_BENCHMARK(Bm_function_Srand48_Lrand48);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Putenv_Unsetenv, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Wcstombs);
MUSL_BENCHMARK(Bm_function_cxa_atexit);
MUSL_BENCHMARK(Bm_function_Srandom);

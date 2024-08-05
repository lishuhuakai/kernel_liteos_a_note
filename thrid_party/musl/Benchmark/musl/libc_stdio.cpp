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

#include "cstdio"
#include "cstdlib"
#include "cstdarg"
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/file.h"
#include "fcntl.h"
#include "unistd.h"
#include "util.h"

using namespace std;

constexpr int SSCANF_SIZE = 32;
constexpr int BUFFERSIZE = 100;
char g_buffer[BUFFERSIZE];

int MyPrintfVf(FILE *stream, const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vfprintf(stream, format, args);
    va_end(args);
    return (ret);
}

int MyPrintfVs(const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vsprintf(g_buffer, format, args);
    va_end(args);
    return (ret);
}

int MyPrintfVsn(const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vsnprintf(g_buffer, BUFFERSIZE, format, args);
    va_end(args);
    return (ret);
}

static void Bm_function_Fopen_read(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "r");
        if (fp == nullptr) {
            perror("fopen read");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_write(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "w");
        if (fp == nullptr) {
            perror("fopen write");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_append(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "a");
        if (fp == nullptr) {
            perror("fopen append");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_rplus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "r+");
        if (fp == nullptr) {
            perror("fopen r+");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_wplus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "w+");
        if (fp == nullptr) {
            perror("fopen w+");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_append_plus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "a+");
        if (fp == nullptr) {
            perror("fopen a+");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_rb(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "rb");
        if (fp == nullptr) {
            perror("fopen rb");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_wb(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "wb");
        if (fp == nullptr) {
            perror("fopen wb");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_ab(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "ab");
        if (fp == nullptr) {
            perror("fopen ab");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_rb_plus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "rb+");
        if (fp == nullptr) {
            perror("fopen rb+");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_wb_plus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "wb+");
        if (fp == nullptr) {
            perror("fopen wb+");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fopen_ab_plus(benchmark::State &state)
{
    for (auto _ : state) {
        FILE *fp = fopen("/dev/zero", "ab+");
        if (fp == nullptr) {
            perror("fopen ab+");
        }
        benchmark::DoNotOptimize(fp);
        state.PauseTiming();
        if (fp != nullptr) {
            fclose(fp);
        }
        state.ResumeTiming();
    }
}

static void Bm_function_Fclose(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        FILE *fp = fopen("/dev/zero", "w+");
        if (fp == nullptr) {
            perror("fclose proc");
        }
        if (fp != nullptr) {
            state.ResumeTiming();
            benchmark::DoNotOptimize(fclose(fp));
        }
    }
}

// Used to convert a file descriptor to a file pointer
static void Bm_function_Fdopen(benchmark::State &state)
{
    for (auto _ : state) {
        int fp = open("/dev/zero", O_RDONLY, OPEN_MODE);
        FILE *fd = fdopen(fp, "r");
        if (fd == nullptr) {
            perror("fdopen");
        }
        benchmark::DoNotOptimize(fd);
        fclose(fd);
    }

    state.SetBytesProcessed(state.iterations());
}

// Use the parameter list to send formatted output to the stream.
// string type
static void Bm_function_Vfprintf_str(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    const char *arr1 = "hello";
    const char *arr2 = "world";
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %s %s success", arr1, arr2));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// int type
static void Bm_function_Vfprintf_int(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    int arr1 = 233;
    int arr2 = 322;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %d %d success", arr1, arr2));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// float type
static void Bm_function_Vfprintf_float(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    float i = 22.33f;
    float j = 33.22f;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %f %f success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// longdouble type
static void Bm_function_Vfprintf_longdouble(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    long double i = 2250996946.3365252546L;
    long double j = 9583454321234.226342465121L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %Lf %Lf success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// unsigned type
static void Bm_function_Vfprintf_unsigned(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    unsigned int i = 4294967295U;
    unsigned int j = 3456264567U;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %u %u success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// long type
static void Bm_function_Vfprintf_long(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    long i = 1234567890L;
    long j = 954611731L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %ld %ld success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// short type
static void Bm_function_Vfprintf_short(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    short i = 32767;
    short j = -32768;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %hd %hd success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// char type
static void Bm_function_Vfprintf_char(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    char i = 'n';
    char j = 'Z';
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVf(fp, "Set parameter %c %c success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// Use the parameter list to send formatted output to the stream.
// string type
static void Bm_function_Fprintf_str(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    const char *arr1 = "hello";
    const char *arr2 = "world";
    for (auto _ : state) {
        benchmark::DoNotOptimize(fprintf(fp, "Set parameter %s %s success", arr1, arr2));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// int type
static void Bm_function_Fprintf_int(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    int arr1 = 233;
    int arr2 = 322;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fprintf(fp, "Set parameter %d %d success", arr1, arr2));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// float type
static void Bm_function_Fprintf_float(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    float i = 22.33f;
    float j = 33.22f;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fprintf(fp, "Set parameter %f %f success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// longdouble type
static void Bm_function_Fprintf_longdouble(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    long double i = 2250996946.3365252546L;
    long double j = 9583454321234.226342465121L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fprintf(fp, "Set parameter %Lf %Lf success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// unsigned type
static void Bm_function_Fprintf_unsigned(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    unsigned int i = 4294967295U;
    unsigned int j = 3456264567U;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fprintf(fp, "Set parameter %u %u success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// long type
static void Bm_function_Fprintf_long(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    long i = 1234567890L;
    long j = 954611731L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fprintf(fp, "Set parameter %ld %ld success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// short type
static void Bm_function_Fprintf_short(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    short i = 32767;
    short j = -32768;
    for (auto _ : state) {
        benchmark::DoNotOptimize(fprintf(fp, "Set parameter %hd %hd success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// char type
static void Bm_function_Fprintf_char(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    char i = 'n';
    char j = 'Z';
    for (auto _ : state) {
        benchmark::DoNotOptimize(fprintf(fp, "Set parameter %c %c success", i, j));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// Use the parameter list to send formatted output to a string
// string type
static void Bm_function_Vsprintf_str(benchmark::State &state)
{
    const char *arr = "signal_stack";
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%s", arr));
    }

    state.SetBytesProcessed(state.iterations());
}

// int type
static void Bm_function_Vsprintf_int(benchmark::State &state)
{
    int i = 2233;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%d", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// float type
static void Bm_function_Vsprintf_float(benchmark::State &state)
{
    float i = 22.33;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%f", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// longdouble type
static void Bm_function_Vsprintf_longdouble(benchmark::State &state)
{
    long double i = 9583454321234.226342465121L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%Lf", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// unsigned type
static void Bm_function_Vsprintf_unsigned(benchmark::State &state)
{
    unsigned int u = 4294967295U;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%u", u));
    }

    state.SetBytesProcessed(state.iterations());
}

// long type
static void Bm_function_Vsprintf_long(benchmark::State &state)
{
    long l = 1234567890L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%ld", l));
    }

    state.SetBytesProcessed(state.iterations());
}

// short type
static void Bm_function_Vsprintf_short(benchmark::State &state)
{
    short s = 32767;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%hd", s));
    }

    state.SetBytesProcessed(state.iterations());
}

// char type
static void Bm_function_Vsprintf_char(benchmark::State &state)
{
    char c = 'Z';
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVs("%c", c));
    }

    state.SetBytesProcessed(state.iterations());
}

// Prints a format string to a string buffer and can limit the maximum length of the formatted string that is printed
// string type
static void Bm_function_Vsnprintf_str(benchmark::State &state)
{
    const char *i = "holy";
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %s", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// int type
static void Bm_function_Vsnprintf_int(benchmark::State &state)
{
    int i = 2233;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %d", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// float type
static void Bm_function_Vsnprintf_float(benchmark::State &state)
{
    float i = 22.33;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %f", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// longdouble type
static void Bm_function_Vsnprintf_longdouble(benchmark::State &state)
{
    long double i = 23423523.769563665L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %Lf", i));
    }

    state.SetBytesProcessed(state.iterations());
}

// unsigned type
static void Bm_function_Vsnprintf_unsigned(benchmark::State &state)
{
    unsigned int u = 4294967295U;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %u", u));
    }

    state.SetBytesProcessed(state.iterations());
}

// long type
static void Bm_function_Vsnprintf_long(benchmark::State &state)
{
    long l = 1234567890L;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %ld", l));
    }

    state.SetBytesProcessed(state.iterations());
}

// short type
static void Bm_function_Vsnprintf_short(benchmark::State &state)
{
    short s = 32767;
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %hd", s));
    }

    state.SetBytesProcessed(state.iterations());
}

// char type
static void Bm_function_Vsnprintf_char(benchmark::State &state)
{
    char s = 'R';
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyPrintfVsn("Error loading shared library %c", s));
    }

    state.SetBytesProcessed(state.iterations());
}

// Wait for the file to no longer be locked by another thread, then make the current thread the owner of the file
// and then increase the lock count and decrease the lock count
static void Bm_function_Flock_Funlockfile(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "r");
    if (fp == nullptr) {
        perror("fopen funlockfile");
    }
    for (auto _ : state) {
        flockfile(fp);
        funlockfile(fp);
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Flock(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDONLY);
    if (fd == -1) {
        perror("open flock");
    }
    for (auto _ : state) {
        flock(fd, LOCK_EX);
        flock(fd, LOCK_UN);
    }
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Rename(benchmark::State &state)
{
    mkdir(DATA_ROOT"/data/data/test_rename", S_IRWXU);
    char oldname[32] = DATA_ROOT"/data/data/test_rename";
    char newname[32] = "test_newname";
    for (auto _ : state) {
        benchmark::DoNotOptimize(rename(oldname, newname));
    }
    rmdir(newname);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fseek_set(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseek set");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseek(f, 0, SEEK_SET));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fseeko_set(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseeko set");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseeko(f, 0, SEEK_SET));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

constexpr int OFFSET_SIZE = 10L;
static void Bm_function_Fseek_cur(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseek cur");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseek(f, OFFSET_SIZE, SEEK_CUR));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fseek_end(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseek end");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseek(f, -OFFSET_SIZE, SEEK_END));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fseeko_cur(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseeko cur");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseeko(f, OFFSET_SIZE, SEEK_CUR));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fseeko_end(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseeko end");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fseeko(f, -OFFSET_SIZE, SEEK_END));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Sscanf_int(benchmark::State &state)
{
    for (auto _ : state) {
        int year, month, day;
        benchmark::DoNotOptimize(sscanf("20230515", "%04d%02d%02d", &year, &month, &day));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_double(benchmark::State &state)
{
    double longitude;
    double latitude;
    for (auto _ : state) {
        benchmark::DoNotOptimize(sscanf("113.123456789 31.123456789", "%lf %lf", &longitude, &latitude));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_char(benchmark::State &state)
{
    for (auto _ : state) {
        char str[SSCANF_SIZE] = "";
        benchmark::DoNotOptimize(sscanf("123456abcdedf", "%31[0-9]", str));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_char1(benchmark::State &state)
{
    for (auto _ : state) {
        char str[SSCANF_SIZE] = "";
        benchmark::DoNotOptimize(sscanf("test/unique_11@qq.com", "%*[^/]/%[^@]", str));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_str(benchmark::State &state)
{
    for (auto _ : state) {
        char str[SSCANF_SIZE] = "";
        benchmark::DoNotOptimize(sscanf("123456abcdedfBCDEF", "%[1-9A-Z]", str));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Sscanf_str1(benchmark::State &state)
{
    for (auto _ : state) {
        char str[SSCANF_SIZE] = "";
        benchmark::DoNotOptimize(sscanf("test TEST", "%*s%s", str));
    }
    state.SetBytesProcessed(state.iterations());
}

int MyScanf1(FILE *stream, const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vfscanf(stream, format, args);
    va_end(args);
    return (ret);
}

int MyVfscanf(const char* str, const char *format, ...)
{
    va_list args;
    int ret;
    va_start(args, format);
    ret = vsscanf(str, format, args);
    va_end(args);
    return (ret);
}

static void Bm_function_Vfscanf_str(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_str.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf str");
    }
    if (fprintf(stream, "%s", "vfscanfStrTest") < 0) {
        perror("fprintf vfscanf str");
    }
    for (auto _ : state) {
        char str[1024] = {'0'};
        benchmark::DoNotOptimize(MyScanf1(stream, "%s", str));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_str.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_int(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_int.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf int");
    }
    int testNumber = 123;
    if (fprintf(stream, "%d", testNumber) < 0) {
        perror("fprintf vfscanf int");
    }
    for (auto _ : state) {
        int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%d", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_int.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_double(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_double.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf double");
    }
    double testNumber = 123.4567;
    if (fprintf(stream, "%lf", testNumber) < 0) {
        perror("fprintf vfscanf double");
    }
    for (auto _ : state) {
        int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%d", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_double.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_float(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_float.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf float");
    }
    double testNumber = 40.0;
    if (fprintf(stream, "%f", testNumber) < 0) {
        perror("fprintf vfscanf float");
    }
    for (auto _ : state) {
        float val = 0.0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%f", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_float.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_char(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_char.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf char");
    }
    if (fprintf(stream, "%c", 'a') < 0) {
        perror("fprintf vfscanf char");
    }
    for (auto _ : state) {
        char val = ' ';
        benchmark::DoNotOptimize(MyScanf1(stream, "%c", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_char.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_iformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_iformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf iformat");
    }
    if (fprintf(stream, "%i", -1) < 0) {
        perror("fprintf vfscanf iformat");
    }
    for (auto _ : state) {
        signed int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%i", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_iformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_oformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_oformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf oformat");
    }
    int testNumber = 0123;
    if (fprintf(stream, "%o", testNumber) < 0) {
        perror("fprintf vfscanf oformat");
    }
    for (auto _ : state) {
        unsigned int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%o", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_oformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_uformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_uformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf uformat");
    }
    int testNumber = 1024;
    if (fprintf(stream, "%u", testNumber) < 0) {
        perror("fprintf vfscanf uformat");
    }
    for (auto _ : state) {
        unsigned int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%u", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_uformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_xformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_xformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf xformat");
    }
    if (fprintf(stream, "%x", 0xabc) < 0) {
        perror("fprintf vfscanf xformat");
    }
    for (auto _ : state) {
        int val = 0x000;
        benchmark::DoNotOptimize(MyScanf1(stream, "%x", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_xformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_Xformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_Xformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf Xformat");
    }
    if (fprintf(stream, "%X", 0xabc) < 0) {
        perror("fprintf vfscanf Xformat");
    }
    for (auto _ : state) {
        int val = 0x000;
        benchmark::DoNotOptimize(MyScanf1(stream, "%X", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_Xformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_eformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_eformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf eformat");
    }
    float testNumber = 123456.0;
    if (fprintf(stream, "%.2e", testNumber) < 0) {
        perror("fprintf vfscanf eformat");
    }
    for (auto _ : state) {
        float val = 0.0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%.2e", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_eformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_gformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_gformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf gformat");
    }
    float testNumber = 12.3;
    if (fprintf(stream, "%g", testNumber) < 0) {
        perror("fprintf vfscanf gformat");
    }
    for (auto _ : state) {
        float val = 12.3;
        benchmark::DoNotOptimize(MyScanf1(stream, "%g", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_gformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_ldformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_gformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf ldformat");
    }
    if (fprintf(stream, "%ld", 2147483646L) < 0) {
        perror("fprintf vfscanf ldformat");
    }
    for (auto _ : state) {
        long int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%ld", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_ldformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_luformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_luformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf luformat");
    }
    if (fprintf(stream, "%lu", 4294967294UL) < 0) {
        perror("fprintf vfscanf luformat");
    }
    for (auto _ : state) {
        unsigned long val = 0UL;
        benchmark::DoNotOptimize(MyScanf1(stream, "%lu", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_luformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_lxformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_lxformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf lxformat");
    }
    if (fprintf(stream, "%lx", 0x41L) < 0) {
        perror("fprintf vfscanf lxformat");
    }
    for (auto _ : state) {
        long val = 0x0L;
        benchmark::DoNotOptimize(MyScanf1(stream, "%lx", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_lxformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_loformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_loformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf loformat");
    }
    if (fprintf(stream, "%lo", 0234L) < 0) {
        perror("fprintf vfscanf loformat");
    }
    for (auto _ : state) {
        long val = 00L;
        benchmark::DoNotOptimize(MyScanf1(stream, "%lo", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_loformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_hdformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_hdformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf hdformat");
    }
    if (fprintf(stream, "%hd", static_cast<short>(144)) < 0) {
        perror("fprintf vfscanf hdformat");
    }
    for (auto _ : state) {
        short int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%hd", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_hdformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_huformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_huformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf huformat");
    }
    if (fprintf(stream, "%hu", static_cast<unsigned short>(256)) < 0) {
        perror("fprintf vfscanf huformat");
    }
    for (auto _ : state) {
        unsigned short int val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%hu", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_huformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_hhuformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_hhuformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf hhuformat");
    }
    if (fprintf(stream, "%hhu", static_cast<unsigned char>(256)) < 0) {
        perror("fprintf vfscanf hhuformat");
    }
    for (auto _ : state) {
        unsigned char val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%hhu", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_hhuformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_hhxformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_hhxformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf hhxformat");
    }
    if (fprintf(stream, "%hhx", static_cast<unsigned char>(0x23)) < 0) {
        perror("fprintf vfscanf hhxformat");
    }
    for (auto _ : state) {
        short val = 0x0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%hhx", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_hhxformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_llxformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_llxformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf llxformat");
    }
    if (fprintf(stream, "%llx", 0x6543LL) < 0) {
        perror("fprintf vfscanf llxformat");
    }
    for (auto _ : state) {
        long long val = 0x0LL;
        benchmark::DoNotOptimize(MyScanf1(stream, "%llx", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_llxformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_lldformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_lldformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf lldformat");
    }
    if (fprintf(stream, "%lld", -23234534LL) < 0) {
        perror("fprintf vfscanf lldformat");
    }
    for (auto _ : state) {
        long long val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%lld", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_lldformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Vfscanf_lluformat(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_lluformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf lluformat");
    }
    if (fprintf(stream, "%llu", 23234534LL) < 0) {
        perror("fprintf vfscanf lluformat");
    }
    for (auto _ : state) {
        unsigned long long val = 0;
        benchmark::DoNotOptimize(MyScanf1(stream, "%llu", &val));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_lluformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fileno_unlocked(benchmark::State &state)
{
    FILE *stream = fopen(DATA_ROOT"/data/data/vfscanf_lluformat.txt", "w+");
    if (stream == nullptr) {
        perror("fopen vfscanf lluformat");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fileno_unlocked(stream));
    }
    fclose(stream);
    remove(DATA_ROOT"/data/data/vfscanf_lluformat.txt");
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fseek_fflush(benchmark::State &state)
{
    FILE *f = fopen("/dev/zero", "r");
    if (f == nullptr) {
        perror("fopen fseek set");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(fflush(f));
    }
    fclose(f);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Sscanf_vsscanf_int(benchmark::State &state)
{
    int year;
    int month;
    int day;
    const char* src = "20230515";
    for (auto _ : state) {
        benchmark::DoNotOptimize(MyVfscanf(src, "%04d%02d%02d", &year, &month, &day));
    }
}

static void Bm_function_Feof(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "r");
    if (fp == nullptr) {
        perror("feof");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(feof(fp));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// Push the char character into the specified stream stream,
// so that it is the next character to be read
static void Bm_function_Ungetc(benchmark::State &state)
{
    int c;
    FILE *fp = fopen("/dev/zero", "r");
    if (fp == nullptr) {
        perror("ungetc open");
    }
    while (state.KeepRunning()) {
        c = fgetc(fp);
        ungetc(c, fp);
    }
    fclose(fp);
}

static void Bm_function_Setbuf(benchmark::State &state)
{
    FILE *stream = stdout;

    const size_t nbytes = state.range(0);
    const size_t bufAlignment = state.range(1);

    vector<char> src;
    char *bufAligned = GetAlignedPtr(&src, bufAlignment, nbytes);
    for (auto _ : state) {
        setbuf(stream, bufAligned);
    }
    setbuf(stream, nullptr);
}

static void Bm_function_Getchar(benchmark::State &state)
{
    FILE *fp = freopen("/dev/zero", "r", stdin);
    for (auto _ : state) {
        benchmark::DoNotOptimize(getchar());
    }
    fclose(fp);
}

static void Bm_function_Fputc(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    if (fp == nullptr) {
        perror("fopen fputc");
    }
    char c = 'Z';
    for (auto _ : state) {
        benchmark::DoNotOptimize(fputc(c, fp));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fputs(benchmark::State &state)
{
    FILE *fp = fopen("/dev/zero", "w+");
    if (fp == nullptr) {
        perror("fopen fputs");
    }
    char str[BUFFERSIZE] = "asdhfdf";
    for (auto _ : state) {
        benchmark::DoNotOptimize(fputs(str, fp));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}

// Used to get the number of bytes offset from the beginning of the file
// position pointer from the current position of the file
static void Bm_function_Ftell(benchmark::State &state)
{
    FILE* fp = fopen("/dev/zero", "w+");
    if (fp == nullptr) {
        perror("fopen ftell");
    }
    fseek(fp, 0, SEEK_END);
    for (auto _ : state) {
        benchmark::DoNotOptimize(ftell(fp));
    }
    fclose(fp);
    state.SetBytesProcessed(state.iterations());
}
#if not defined __APPLE__
static void Bm_function_fread_unlocked(benchmark::State& state)
{
    FILE *fp = fopen("/dev/zero", "r");
    if (fp == nullptr) {
        perror("fopen read");
    }
    int n = 256;
    char* buf = new char[n];
    for (auto _ : state) {
        benchmark::DoNotOptimize(fread_unlocked(buf, n, 1, fp));
    }
    delete[] buf;
    fclose(fp);
}

static void Bm_function_fgets_unlocked(benchmark::State& state)
{
    FILE *fp = fopen("/dev/zero", "r");
    if (fp == nullptr) {
        perror("fopen read");
    }
    int n = 256;
    char* buf = new char[n];
    for (auto _ : state) {
        benchmark::DoNotOptimize(fgets_unlocked(buf, n, fp));
    }
    delete[] buf;
    fclose(fp);
}
#endif

// Rename files and directories
static void Bm_function_Renameat(benchmark::State &state)
{
    const char *oldPath = DATA_ROOT"/data/data/test_old_renameat.txt";
    int oldFd = open(oldPath, O_RDWR | O_CREAT, OPEN_MODE);
    if (oldFd == -1) {
        perror("open renameat old");
    }
    close(oldFd);
    const char *newPath = DATA_ROOT"/data/data/test_new_renameat.txt";
    int newFd = open(newPath, O_RDWR | O_CREAT, OPEN_MODE);
    if (newFd == -1) {
        perror("open renameat new");
    }
    close(newFd);
    for (auto _ : state) {
        benchmark::DoNotOptimize(renameat(oldFd, oldPath, newFd, newPath));
    }
    remove(newPath);
    state.SetBytesProcessed(state.iterations());
}

static void BM_function_Snprintf_d4(benchmark::State& state)
{
    char buf[BUFSIZ];
    char a[4] = {127, 0, 0, 1};
    while (state.KeepRunning()) {
        benchmark::DoNotOptimize(snprintf(buf, sizeof(buf), "%d.%d.%d.%d", a[0], a[1], a[2], a[3]));
    }
}

#if not defined __APPLE__
MUSL_BENCHMARK(Bm_function_fgets_unlocked);
MUSL_BENCHMARK(Bm_function_fread_unlocked);
#endif
MUSL_BENCHMARK(Bm_function_Fopen_read);
MUSL_BENCHMARK(Bm_function_Fopen_write);
MUSL_BENCHMARK(Bm_function_Fopen_append);
MUSL_BENCHMARK(Bm_function_Fopen_rplus);
MUSL_BENCHMARK(Bm_function_Fopen_wplus);
MUSL_BENCHMARK(Bm_function_Fopen_append_plus);
MUSL_BENCHMARK(Bm_function_Fopen_rb);
MUSL_BENCHMARK(Bm_function_Fopen_wb);
MUSL_BENCHMARK(Bm_function_Fopen_ab);
MUSL_BENCHMARK(Bm_function_Fopen_rb_plus);
MUSL_BENCHMARK(Bm_function_Fopen_wb_plus);
MUSL_BENCHMARK(Bm_function_Fopen_ab_plus);
MUSL_BENCHMARK(Bm_function_Fclose);
MUSL_BENCHMARK(Bm_function_Fdopen);
MUSL_BENCHMARK(Bm_function_Vfprintf_str);
MUSL_BENCHMARK(Bm_function_Vfprintf_int);
MUSL_BENCHMARK(Bm_function_Vfprintf_float);
MUSL_BENCHMARK(Bm_function_Vfprintf_longdouble);
MUSL_BENCHMARK(Bm_function_Vfprintf_unsigned);
MUSL_BENCHMARK(Bm_function_Vfprintf_long);
MUSL_BENCHMARK(Bm_function_Vfprintf_short);
MUSL_BENCHMARK(Bm_function_Vfprintf_char);
MUSL_BENCHMARK(Bm_function_Fprintf_str);
MUSL_BENCHMARK(Bm_function_Fprintf_int);
MUSL_BENCHMARK(Bm_function_Fprintf_float);
MUSL_BENCHMARK(Bm_function_Fprintf_longdouble);
MUSL_BENCHMARK(Bm_function_Fprintf_unsigned);
MUSL_BENCHMARK(Bm_function_Fprintf_long);
MUSL_BENCHMARK(Bm_function_Fprintf_short);
MUSL_BENCHMARK(Bm_function_Fprintf_char);
MUSL_BENCHMARK(Bm_function_Vsprintf_str);
MUSL_BENCHMARK(Bm_function_Vsprintf_int);
MUSL_BENCHMARK(Bm_function_Vsprintf_float);
MUSL_BENCHMARK(Bm_function_Vsprintf_longdouble);
MUSL_BENCHMARK(Bm_function_Vsprintf_unsigned);
MUSL_BENCHMARK(Bm_function_Vsprintf_long);
MUSL_BENCHMARK(Bm_function_Vsprintf_short);
MUSL_BENCHMARK(Bm_function_Vsprintf_char);
MUSL_BENCHMARK(Bm_function_Vsnprintf_str);
MUSL_BENCHMARK(Bm_function_Vsnprintf_int);
MUSL_BENCHMARK(Bm_function_Vsnprintf_float);
MUSL_BENCHMARK(Bm_function_Vsnprintf_longdouble);
MUSL_BENCHMARK(Bm_function_Vsnprintf_unsigned);
MUSL_BENCHMARK(Bm_function_Vsnprintf_long);
MUSL_BENCHMARK(Bm_function_Vsnprintf_short);
MUSL_BENCHMARK(Bm_function_Vsnprintf_char);
MUSL_BENCHMARK(Bm_function_Flock_Funlockfile);
MUSL_BENCHMARK(Bm_function_Flock);
MUSL_BENCHMARK(Bm_function_Rename);
MUSL_BENCHMARK(Bm_function_Fseek_set);
MUSL_BENCHMARK(Bm_function_Fseek_cur);
MUSL_BENCHMARK(Bm_function_Fseek_end);
MUSL_BENCHMARK(Bm_function_Fseeko_set);
MUSL_BENCHMARK(Bm_function_Fseeko_cur);
MUSL_BENCHMARK(Bm_function_Fseeko_end);
MUSL_BENCHMARK(Bm_function_Sscanf_int);
MUSL_BENCHMARK(Bm_function_Sscanf_double);
MUSL_BENCHMARK(Bm_function_Sscanf_str);
MUSL_BENCHMARK(Bm_function_Sscanf_str1);
MUSL_BENCHMARK(Bm_function_Sscanf_char);
MUSL_BENCHMARK(Bm_function_Sscanf_char1);
MUSL_BENCHMARK(Bm_function_Vfscanf_str);
MUSL_BENCHMARK(Bm_function_Vfscanf_int);
MUSL_BENCHMARK(Bm_function_Vfscanf_double);
MUSL_BENCHMARK(Bm_function_Vfscanf_float);
MUSL_BENCHMARK(Bm_function_Vfscanf_char);
MUSL_BENCHMARK(Bm_function_Vfscanf_iformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_oformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_uformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_xformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_Xformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_eformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_gformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_ldformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_luformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_lxformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_loformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_hdformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_huformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_hhuformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_hhxformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_llxformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_lldformat);
MUSL_BENCHMARK(Bm_function_Vfscanf_lluformat);
MUSL_BENCHMARK(Bm_function_Fileno_unlocked);
MUSL_BENCHMARK(Bm_function_Fseek_fflush);
MUSL_BENCHMARK(Bm_function_Sscanf_vsscanf_int);
MUSL_BENCHMARK(Bm_function_Feof);
MUSL_BENCHMARK(Bm_function_Ungetc);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Setbuf, "ALIGNED_ONEBUF");
MUSL_BENCHMARK(Bm_function_Getchar);
MUSL_BENCHMARK(Bm_function_Fputc);
MUSL_BENCHMARK(Bm_function_Fputs);
MUSL_BENCHMARK(Bm_function_Ftell);
MUSL_BENCHMARK(Bm_function_Renameat);
MUSL_BENCHMARK(BM_function_Snprintf_d4);

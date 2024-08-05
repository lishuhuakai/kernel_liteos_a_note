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

#include <sys/syscall.h>
#include <cstring>
#include <sys/timex.h>
#include <csignal>
#include <sys/resource.h>
#include <cstdio>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
#include <sys/utsname.h>
#if not defined __APPLE__
#include <sys/eventfd.h>
#endif
#include "util.h"

using namespace std;

constexpr int BYTES_WRITTEN = 8;
constexpr int BUFSIZE = 10;

// Gets the identification code of the current process
static void Bm_function_Syscall_getpid(benchmark::State &state)
{
    pid_t pid;
    for (auto _ : state) {
        pid = syscall(SYS_getpid);
        benchmark::DoNotOptimize(pid);
    }
}

// The system call gets the current thread ID
static void Bm_function_Syscall_gettid(benchmark::State &state)
{
    pid_t tid;
    for (auto _ : state) {
        tid = syscall(SYS_gettid);
        benchmark::DoNotOptimize(tid);
    }
}

// Used to read the kernel time
static void Bm_function_Syscall_adjtimex(benchmark::State &state)
{
    struct timex timeInfo;
    for (auto _ : state) {
        benchmark::DoNotOptimize(syscall(SYS_adjtimex, &timeInfo));
    }
}

// Writes the contents of the file of the user buffer to the corresponding location of the file on disk
static void Bm_function_Syscall_write(benchmark::State &state)
{
    int fp = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fp == -1) {
        perror("open SYS_write");
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(syscall(SYS_write, fp, "Bad Mind", BYTES_WRITTEN));
    }
    close(fp);
}

// Transfer 10 bytes into the BUFF in the open file
static void Bm_function_Syscall_read(benchmark::State &state)
{
    char buf[BUFSIZE];
    int fd = open("/dev/zero", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open SYS_read");
    }
    for (auto _ : state) {
        ssize_t ret = syscall(SYS_read, fd, buf, BUFSIZE);
        if (ret < 0) {
            perror("STS_read");
        }
        benchmark::DoNotOptimize(ret);
    }
    close(fd);
}

// Manipulate the characteristics of a file based on the file descriptor
static void Bm_function_Syscall_fcntl(benchmark::State &state)
{
    long int ret;
    int fd = open("/dev/zero", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open SYS_fcntl");
    }
    for (auto _ : state) {
        ret = syscall(SYS_fcntl, fd, F_GETFL);
        if (ret == -1) {
            perror("SYS_fcntl");
        }
        benchmark::DoNotOptimize(ret);
    }
    close(fd);
}

// Obtain system resource usage status
static void Bm_function_Syscall_getrusage(benchmark::State &state)
{
    struct rusage usage;
    long result;
    for (auto _ : state) {
        result = syscall(SYS_getrusage, RUSAGE_SELF, &usage);
        benchmark::DoNotOptimize(result);
    }
    if (result != 0) {
        printf("SYS_getrusage error\n");
    }
}

// Obtain system information
static void Bm_function_Syscall_uname(benchmark::State &state)
{
    struct utsname buf;
    long int ret;
    for (auto _ : state) {
        ret = syscall(SYS_uname, &buf);
        benchmark::DoNotOptimize(ret);
    }
    if (ret != 0) {
        printf("SYS_uname error\n");
    }
}

static void Bm_function_Syscall_getpriority(benchmark::State &state)
{
    pid_t pid = getpid();
    for (auto _ : state) {
        benchmark::DoNotOptimize(getpriority(PRIO_PROCESS, pid));
    }
}

static void Bm_function_Syscall_setpriority(benchmark::State &state)
{
    pid_t pid = getpid();
    int prio = getpriority(PRIO_PROCESS, pid);
    if (prio == -1) {
        perror("getpriority failed");
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(setpriority(PRIO_PROCESS, pid, prio));
    }
}

static void Bm_function_Syscall_eventfd(benchmark::State &state)
{
    for (auto _ : state) {
        int fd = eventfd(0, 0);
        if (fd >= 0) {
            state.PauseTiming();
            close(fd);
            state.ResumeTiming();
        }
    }
}

MUSL_BENCHMARK(Bm_function_Syscall_getpid);
MUSL_BENCHMARK(Bm_function_Syscall_gettid);
MUSL_BENCHMARK(Bm_function_Syscall_adjtimex);
MUSL_BENCHMARK(Bm_function_Syscall_write);
MUSL_BENCHMARK(Bm_function_Syscall_read);
MUSL_BENCHMARK(Bm_function_Syscall_fcntl);
MUSL_BENCHMARK(Bm_function_Syscall_getrusage);
MUSL_BENCHMARK(Bm_function_Syscall_uname);
MUSL_BENCHMARK(Bm_function_Syscall_getpriority);
MUSL_BENCHMARK(Bm_function_Syscall_setpriority);
MUSL_BENCHMARK(Bm_function_Syscall_eventfd);

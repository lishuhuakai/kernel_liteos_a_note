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
#include "unistd.h"
#include "cstdio"
#include "dirent.h"
#include "ctime"
#include "util.h"

using namespace std;

constexpr int LOCK_SIZE = 10;
constexpr int TIME_SIZE = 2;
static void Bm_function_Fcntl_getfl(benchmark::State &state)
{
    int fd = open("/etc/passwd", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open fcntl_getfl");
    }
    for (auto _ : state) {
        int ret = fcntl(fd, F_GETFL);
        if (ret < 0) {
            perror("fcntl_getfl proc");
        }
        benchmark::DoNotOptimize(ret);
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fcntl_setfl(benchmark::State &state)
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (flags < 0) {
        perror("fcntl_setfl F_GETFL");
    }
    flags |= O_NONBLOCK;
    for (auto _ : state) {
        int ret = fcntl(STDIN_FILENO, F_SETFL, flags);
        if (ret < 0) {
            perror("fcntl_setfl proc");
        }
        benchmark::DoNotOptimize(ret);

        state.PauseTiming();
        flags &= ~O_NONBLOCK;
        if (fcntl(STDOUT_FILENO, F_SETFL, flags) < -1) {
            perror("fcntl_setfl proc");
        }
        state.ResumeTiming();
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fcntl_setlkw(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open fcntl_setlkw");
    }
    struct flock fdLock;
    for (auto _ : state) {
        state.PauseTiming();
        fdLock.l_type = F_WRLCK;
        fdLock.l_whence = SEEK_SET;
        fdLock.l_start = 0;
        fdLock.l_len = LOCK_SIZE;
        if (fcntl(fd, F_SETLKW, &fdLock) < 0) {
            perror("fcntl_setlkw F_WRLCK");
        }
        fdLock.l_type = F_UNLCK;
        state.ResumeTiming();
        int ret = fcntl(fd, F_SETLKW, &fdLock);
        if (ret < 0) {
            perror("fcntl_setlkw proc");
        }
        benchmark::DoNotOptimize(ret);
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fcntl_dupfd(benchmark::State &state)
{
    int fd = open("/etc/passwd", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open fcntl_dupfd");
    }
    for (auto _ : state) {
        int dupfd = fcntl(fd, F_DUPFD, 0);
        if (dupfd < 0) {
            perror("fcntl_dupfd proc");
        }
        benchmark::DoNotOptimize(dupfd);
        state.PauseTiming();
        close(dupfd);
        state.ResumeTiming();
    }
    close(fd);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Fcntl_setlk(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open fcntl_setlk");
    }
    struct flock fdLock;
    for (auto _ : state) {
        state.PauseTiming();
        fdLock.l_type = F_WRLCK;
        fdLock.l_whence = SEEK_SET;
        fdLock.l_start = 0;
        fdLock.l_len = LOCK_SIZE;
        if (fcntl(fd, F_SETLK, &fdLock) < 0) {
            perror("fcntl_setlk F_WRLCK");
        }
        fdLock.l_type = F_UNLCK;
        state.ResumeTiming();
        int ret = fcntl(fd, F_SETLK, &fdLock);
        if (ret < 0) {
            perror("fcntl_setlk proc");
        }
        benchmark::DoNotOptimize(ret);
    }
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fcntl_getlk(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open fcntl_getlk");
    }
    struct flock fdLock;
    for (auto _ : state) {
        state.PauseTiming();
        fdLock.l_type = F_RDLCK;
        fdLock.l_whence = SEEK_SET;
        fdLock.l_start = 0;
        fdLock.l_len = LOCK_SIZE;
        state.ResumeTiming();
        int ret = fcntl(fd, F_GETLK, &fdLock);
        if (ret < 0) {
            perror("fcntl_getlk proc");
        }
        benchmark::DoNotOptimize(ret);
    }
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fcntl_getfd(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open fcntl_getfd");
    }
    for (auto _ : state) {
        int ret = fcntl(fd, F_GETFD);
        if (ret < 0) {
            perror("fcntl_getfd proc");
        }
        benchmark::DoNotOptimize(ret);
    }
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Fcntl_setfd(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open fcntl_setfd");
    }
    for (auto _ : state) {
        state.PauseTiming();
        int flags = fcntl(fd, F_GETFD);
        if (flags < 0) {
            perror("fcntl_setfd F_GETFD");
        }
        flags |= FD_CLOEXEC;
        state.ResumeTiming();
        int ret = fcntl(fd, F_SETFD, flags);
        if (ret < 0) {
            perror("fcntl_setfd proc");
        }
        benchmark::DoNotOptimize(ret);

        state.PauseTiming();
        flags &= ~FD_CLOEXEC;
        if (fcntl(fd, F_SETFD, flags) < -1) {
            perror("fcntl_setfd F_SETFD");
        }
        state.ResumeTiming();
    }
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Open_rdonly(benchmark::State &state)
{
    const char *filename = "/proc/self/cmdline";
    for (auto _ : state) {
        int fd = open(filename, O_RDONLY, OPEN_MODE);
        if (fd == -1) {
            perror("open_rdonly proc");
        }
        benchmark::DoNotOptimize(fd);
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Open_rdwr(benchmark::State &state)
{
    const char *filename = "/dev/zero";
    for (auto _ : state) {
        int fd = open(filename, O_RDWR, OPEN_MODE);
        if (fd == -1) {
            perror("open_rdwr proc");
        }
        benchmark::DoNotOptimize(fd);
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Open_creat_rdwr(benchmark::State &state)
{
    const char *filename = "/data/log/hiview/sys_event_logger/event.db";
    for (auto _ : state) {
        int fd = open(filename, O_RDWR | O_CREAT, OPEN_MODE);
        if (fd == -1) {
            perror("open_creat_rdwr proc");
        }
        benchmark::DoNotOptimize(fd);
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
    state.SetItemsProcessed(state.iterations());
}

// Used to modify the access time and modification time of a file or directory
// function default behavior
static void Bm_function_Utimensat_Normal(benchmark::State &state)
{
    int dirfd = AT_FDCWD;
    const char* path = "hotspot_function.json";
    struct timespec times[TIME_SIZE];
    clock_gettime(CLOCK_REALTIME, &times[0]);
    times[1] = times[0];
    for (auto _ : state) {
        benchmark::DoNotOptimize(utimensat(dirfd, path, times, AT_EACCESS));
    }
    state.SetItemsProcessed(state.iterations());
}

// avoid dealing with symbolic links
static void Bm_function_Utimensat_Nofollow(benchmark::State &state)
{
    int dirfd = AT_FDCWD;
    const char* path = "hotspot_function.json";
    struct timespec times[TIME_SIZE];
    clock_gettime(CLOCK_REALTIME, &times[0]);
    times[1] = times[0];
    for (auto _ : state) {
        benchmark::DoNotOptimize(utimensat(dirfd, path, times, AT_SYMLINK_NOFOLLOW));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Creat(benchmark::State &state)
{
    const char *filename = "/dev/zero";
    for (auto _ : state) {
        int fd = creat(filename, OPEN_MODE);
        if (fd == -1) {
            perror("creat proc");
        }
        benchmark::DoNotOptimize(fd);
        close(fd);
    }
    state.SetBytesProcessed(state.iterations());
}

// Used to open or create a file
// current directory absolute path read only
static void Bm_function_Openat_rdonly(benchmark::State &state)
{
    const char *filename = "/proc/self/cmdline";
    for (auto _ : state) {
        int fd = openat(AT_FDCWD, filename, O_RDONLY, OPEN_MODE);
        if (fd == -1) {
            perror("openat_rdonly proc");
        }
        benchmark::DoNotOptimize(fd);
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
    state.SetBytesProcessed(state.iterations());
}

// current directory create read write
static void Bm_function_Openat_creat_rdwr(benchmark::State &state)
{
    const char *filename = "/data/log/hiview/sys_event_logger/event.db";
    for (auto _ : state) {
        int fd = openat(AT_FDCWD, filename, O_RDWR | O_CREAT, OPEN_MODE);
        if (fd == -1) {
            perror("openat_creat_rdwr proc");
        }
        benchmark::DoNotOptimize(fd);
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
    state.SetItemsProcessed(state.iterations());
}

// current directory relative path
static void Bm_function_Openat_RelativePath_AT_FDCWD(benchmark::State &state)
{
    const char *filename = "testOpenat.txt";
    for (auto _ : state) {
        int fd = openat(AT_FDCWD, filename, O_RDWR | O_CREAT, OPEN_MODE);
        if (fd == -1) {
            perror("openat_creat_rdwr proc");
        }
        benchmark::DoNotOptimize(fd);
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
    remove(filename);
    state.SetItemsProcessed(state.iterations());
}

// other directories
static void Bm_function_Openat_RelativePath_fd(benchmark::State &state)
{
    const char *filename = "/dev/";
    DIR *dir = opendir(filename);
    int fdDir = dirfd(dir);
    for (auto _ : state) {
        int fd = openat(fdDir, "zero", O_RDWR | O_CREAT, OPEN_MODE);
        if (fd == -1) {
            perror("open_rdwr proc");
        }
        benchmark::DoNotOptimize(fd);
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
    closedir(dir);
    state.SetItemsProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Fcntl_getfl);
MUSL_BENCHMARK(Bm_function_Fcntl_setfl);
MUSL_BENCHMARK(Bm_function_Fcntl_setlkw);
MUSL_BENCHMARK(Bm_function_Fcntl_dupfd);
MUSL_BENCHMARK(Bm_function_Fcntl_setlk);
MUSL_BENCHMARK(Bm_function_Fcntl_getlk);
MUSL_BENCHMARK(Bm_function_Fcntl_getfd);
MUSL_BENCHMARK(Bm_function_Fcntl_setfd);
MUSL_BENCHMARK(Bm_function_Open_rdonly);
MUSL_BENCHMARK(Bm_function_Open_rdwr);
MUSL_BENCHMARK(Bm_function_Open_creat_rdwr);
MUSL_BENCHMARK(Bm_function_Utimensat_Normal);
MUSL_BENCHMARK(Bm_function_Utimensat_Nofollow);
MUSL_BENCHMARK(Bm_function_Creat);
MUSL_BENCHMARK(Bm_function_Openat_rdonly);
MUSL_BENCHMARK(Bm_function_Openat_creat_rdwr);
MUSL_BENCHMARK(Bm_function_Openat_RelativePath_AT_FDCWD);
MUSL_BENCHMARK(Bm_function_Openat_RelativePath_fd);

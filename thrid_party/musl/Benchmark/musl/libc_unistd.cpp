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

#include "unistd.h"
#include "cstddef"
#include "cfenv"
#include "sys/mman.h"
#if not defined __APPLE__
#include "sys/prctl.h"
#endif
#include "sys/types.h"
#include "sys/stat.h"
#include "sys/uio.h"
#include "sys/utsname.h"
#include "fcntl.h"
#include "cstdio"
#include "sys/time.h"
#include "ctime"
#include "err.h"
#include "cstring"
#include "cerrno"
#include "dirent.h"
#include "util.h"

using namespace std;

constexpr int BUFFER = 1024;
constexpr int IOV_SIZE = 2;

static const vector<int> pipe2Flags {
    O_CLOEXEC,
    O_DIRECT,
    O_CLOEXEC | O_DIRECT,
};

static void PrepareArgsPipe2(benchmark::internal::Benchmark* b)
{
    for (auto f : pipe2Flags) {
        b->Args({f});
    }
}

void ReadWriteTest(benchmark::State& state, bool isRead)
{
    size_t chunkSize = state.range(0);
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    if (fd == -1) {
        perror("open ReadWriteTest");
    }
    char* bufRead = new char[chunkSize];
    const char *bufWrite = "hello world!";
    if (isRead) {
        memset(bufRead, 0, chunkSize * sizeof(char));
    }

    while (state.KeepRunning()) {
        if (isRead) {
            if (read(fd, bufRead, chunkSize) == -1) {
                perror("Read proc");
            }
        } else {
            if (write(fd, bufWrite, chunkSize) == -1) {
                perror("write proc");
            }
        }
    }
    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(chunkSize));
    delete[] bufRead;
    close(fd);
}

template <typename Fn>
void PreadWriteTest(benchmark::State &state, Fn f, bool buffered)
{
    size_t chunkSize = state.range(0);
    int fp = open("/dev/zero", O_RDWR, OPEN_MODE);
    char *buf = new char[chunkSize];
    off64_t offset = 0;
    while (state.KeepRunning()) {
        if (f(fp, buf, strlen(buf), offset) == -1) {
            errx(1, "ERROR: op of %zu bytes failed.", chunkSize);
        }
    }

    state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(chunkSize));
    delete[] buf;
    close(fp);
}

// Obtain the identification code of the current process
static void Bm_function_Getpid(benchmark::State &state)
{
    pid_t pid;
    for (auto _ : state) {
        pid = getpid();
        benchmark::DoNotOptimize(pid);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Geteuid(benchmark::State &state)
{
    uid_t uid;
    for (auto _ : state) {
        uid = geteuid();
        benchmark::DoNotOptimize(uid);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Close(benchmark::State &state)
{
    for (auto _ : state) {
        state.PauseTiming();
        int fd = open("/dev/zero", O_RDONLY, OPEN_MODE);
        if (fd == -1) {
            perror("open Close");
        }
        state.ResumeTiming();
        benchmark::DoNotOptimize(close(fd));
    }
    state.SetBytesProcessed(state.iterations());
}

constexpr int SECOND = 50;

static void Bm_function_Usleep(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(usleep(SECOND));
    }
    state.SetItemsProcessed(state.iterations());
}

void Bm_function_Pwrite64(benchmark::State &state)
{
    PreadWriteTest(state, pwrite, true);
}

void Bm_function_Pread64(benchmark::State &state)
{
    PreadWriteTest(state, pread, true);
}

// Stores the destination path of the symbolic link file into a buffer and returns the length of the destination path
static void Bm_function_Readlink(benchmark::State &state)
{
    char path[BUFFER] = {0};
    ssize_t len;
    int i = symlink("/dev/zero", "/data/local/tmp/tmplink");
    if (i == -1) {
        perror("symlink");
        exit(-1);
    }
    for (auto _ : state) {
        len = readlink("/data/local/tmp/tmplink", path, sizeof(path));
        benchmark::DoNotOptimize(len);
    }
    remove("/data/local/tmp/tmplink");
}

static void Bm_function_Readlinkat(benchmark::State &state)
{
    char path[BUFFER] = {0};
    ssize_t len;
    int i = symlink("/dev/zero", "/data/local/tmp/tmplink");
    if (i == -1) {
        perror("symlink");
    }
    DIR *dir = opendir("/data/local/tmp");
    if (dir == nullptr) {
        perror("opendir");
    }
    int fd = dirfd(dir);
    for (auto _ : state) {
        len = readlinkat(fd, "./tmplink", path, sizeof(path));
        benchmark::DoNotOptimize(len);
    }
    closedir(dir);
    remove("/data/local/tmp/tmplink");
}

extern "C" ssize_t __readlinkat_chk(int dirfd, const char*, char*, size_t, size_t);

static void Bm_function_Readlinkat_chk(benchmark::State& state)
{
    char path[BUFFER] = {0};
    ssize_t len;
    int i = symlink("/dev/zero", "/data/local/tmp/tmplink");
    if (i == -1) {
        perror("symlink");
    }
    DIR *dir = opendir("/data/local/tmp");
    if (dir == nullptr) {
        perror("opendir");
    }
    int fd = dirfd(dir);
    for (auto _ : state) {
        len = __readlinkat_chk(fd, "./tmplink", path, sizeof(path), __builtin_object_size(path, 1));
        benchmark::DoNotOptimize(len);
    }
    closedir(dir);
    remove("/data/local/tmp/tmplink");
}

static void Bm_function_Getuid(benchmark::State &state)
{
    uid_t uid;
    for (auto _ : state) {
        uid = getuid();
        benchmark::DoNotOptimize(uid);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Getegid(benchmark::State &state)
{
    gid_t gid;
    for (auto _ : state) {
        gid = getegid();
        benchmark::DoNotOptimize(gid);
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Read(benchmark::State &state)
{
    ReadWriteTest(state, true);
}

static void Bm_function_Write(benchmark::State &state)
{
    ReadWriteTest(state, false);
}

static void Bm_function_Access_exist(benchmark::State &state)
{
    const char *filename = "/data";
    for (auto _ : state) {
        benchmark::DoNotOptimize(access(filename, F_OK));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Access_read(benchmark::State &state)
{
    const char *filename = "/data";
    for (auto _ : state) {
        benchmark::DoNotOptimize(access(filename, R_OK));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Access_write(benchmark::State &state)
{
    const char *filename = "/data";
    for (auto _ : state) {
        benchmark::DoNotOptimize(access(filename, W_OK));
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Access_execute(benchmark::State &state)
{
    const char *filename = "/data";
    for (auto _ : state) {
        benchmark::DoNotOptimize(access(filename, X_OK));
    }
    state.SetBytesProcessed(state.iterations());
}

static const char *g_writevvariable1[] = {"Pretend inferiority and", "hello,",
                                          "non sa module libtoken_sync_manager_service.z.so",
                                          "The variable device_company was",
                                          "but never appeared in a",
                                          "The build continued as if",
                                          "product_name=rk3568", "build/toolchain/ohos:"};
static const char *g_writevvariable2[] = {"encourage others arrogance!", "world!", ":token_sync_manager_service",
                                          "set as a build argument", "declare_args() block in any buildfile",
                                          "that argument was unspecified", "ohos_build_type=", "ohos_clang_arm64"};
// Write the contents of multiple buffers to the file descriptor at once
static void Bm_function_Writev(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDWR, OPEN_MODE);
    const char *str1 = g_writevvariable1[state.range(0)];
    const char *str2 = g_writevvariable2[state.range(0)];
    struct iovec iov[IOV_SIZE];
    ssize_t nwritten;
    iov[0].iov_base = (void *)str1;
    iov[0].iov_len = strlen(str1);
    iov[1].iov_base = (void *)str2;
    iov[1].iov_len = strlen(str2);

    for (auto _ : state) {
        nwritten = writev(fd, iov, IOV_SIZE);
        benchmark::DoNotOptimize(nwritten);
    }
    close(fd);
}

static void Bm_function_Uname(benchmark::State &state)
{
    for (auto _ : state) {
        struct utsname buffer;
        benchmark::DoNotOptimize(uname(&buffer));
    }
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Lseek(benchmark::State &state)
{
    const char *filepath = "/data/local/tmp/lseel_test.txt";
    int fd = open(filepath, O_CREAT | O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open lseek");
    }
    int testNumber = 10;
    const char *bufStr = "lseek_test";
    write(fd, bufStr, testNumber);
    for (auto _ : state) {
        lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
    }
    close(fd);
    remove(filepath);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Dup(benchmark::State &state)
{
    int fd = -1;
    for (auto _ : state) {
        fd = dup(STDOUT_FILENO);
        if (fd == -1) {
            perror("dup");
        }

        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
}

static void Bm_function_Pipe2(benchmark::State &state)
{
    int flags = state.range(0);
    for (auto _ : state) {
        int fd[2];
        if (pipe2(fd, flags) < 0) {
            perror("pipe");
        }

        state.PauseTiming();
        close(fd[0]);
        close(fd[1]);
        state.ResumeTiming();
    }
}

static void Bm_function_Getopt(benchmark::State &state)
{
    for (auto _ : state) {
        int argc = 4;
        char* argv[] = {
            (char*)"exe",
            (char*)"-ab",
            (char*)"-c",
            (char*)"foo"
        };
        int opt = 0;
        while (opt != -1) {
            opt = getopt(argc, argv, "abc:");
        }
    }
}

static void Bm_function_Fsync(benchmark::State &state)
{
    const char *filepath = "/data/local/tmp/fsync_test";
    int fd = open(filepath, O_CREAT | O_WRONLY, OPEN_MODE);
    if (fd < 0) {
        perror("open fsync");
    }
    int testNumber = 4;
    write(fd, "test", testNumber);
    for (auto _ : state) {
        fsync(fd);
    }
    close(fd);
    remove(filepath);
}

static void Bm_function_Fdatasync(benchmark::State &state)
{
    const char *filepath = "/data/local/tmp/fdatasync_test";
    int fd = open(filepath, O_CREAT | O_WRONLY, OPEN_MODE);
    if (fd < 0) {
        perror("open fdatasync");
    }
    int testNumber = 4;
    write(fd, "test", testNumber);
    for (auto _ : state) {
        fdatasync(fd);
    }
    close(fd);
    remove(filepath);
}

static void Bm_function_Ftruncate(benchmark::State &state)
{
    const char *filepath = "/data/local/tmp/ftruncate_test";
    char buf[BUFFER] = {0};
    memset(buf, 'a', BUFFER);
    int fd = open(filepath, O_CREAT | O_WRONLY, OPEN_MODE);
    if (fd < 0) {
        perror("open ftruncate");
    }
    write(fd, buf, BUFFER);
    for (auto _ : state) {
        ftruncate(fd, 0);
    }
    close(fd);
    remove(filepath);
}

static void Bm_function_Unlink(benchmark::State &state)
{
    const char *filepath = "/data/local/tmp/unlink_test";
    int fd = open(filepath, O_CREAT | O_RDWR, OPEN_MODE);
    if (fd < 0) {
        perror("open unlink");
    }
    for (auto _ : state) {
        unlink(filepath);
    }
    close(fd);
}

// Used to check file access permissions
// function default behavior
static void Bm_function_Faccessat_Normal(benchmark::State &state)
{
    int dirfd = AT_FDCWD;
    const char* path = "musl_benchmark";
    int mode = F_OK | R_OK | W_OK | X_OK;
    for (auto _ : state) {
        benchmark::DoNotOptimize(faccessat(dirfd, path, mode, AT_EACCESS));
    }
}

// avoid dealing with symbolic links
static void Bm_function_Faccessat_Nofollow(benchmark::State &state)
{
    int dirfd = AT_FDCWD;
    const char* path = "musl_benchmark";
    int mode = F_OK | R_OK | W_OK | X_OK;
    for (auto _ : state) {
        benchmark::DoNotOptimize(faccessat(dirfd, path, mode, AT_SYMLINK_NOFOLLOW));
    }
}

// Used to read data from a file descriptor into multiple buffers
static void Bm_function_Readv(benchmark::State &state)
{
    int fd = open("/dev/zero", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("readv fail");
    }

    const size_t nbytes = state.range(0);
    const size_t bufferalignment1 = state.range(1);
    const size_t bufferalignment2 = state.range(2);
    std::vector<char> buffer1;
    std::vector<char> buffer2;
    char* iovbuffer1 = GetAlignedPtr(&buffer1, bufferalignment1, nbytes);
    char* iovbuffer2 = GetAlignedPtr(&buffer2, bufferalignment2, nbytes);

    struct iovec iov[IOV_SIZE];
    ssize_t numRead;

    iov[0].iov_base = iovbuffer1;
    iov[0].iov_len = buffer1.size();
    iov[1].iov_base = iovbuffer2;
    iov[1].iov_len = buffer2.size();

    for (auto _ : state) {
        numRead = readv(fd, iov, IOV_SIZE);
        benchmark::DoNotOptimize(numRead);
    }
    close(fd);
}

// System functions that change file owners and groups
static void Bm_function_Chown(benchmark::State &state)
{
    const char *filename = "/data/data/chown.txt";
    int fd = open(filename, O_RDWR | O_CREAT, OPEN_MODE);
    if (fd == -1) {
        perror("open chown");
    }
    for (auto _ : state) {
        if (chown(filename, 1, 1) != 0) {
            perror("chown proc");
        }
    }
    close(fd);
    remove(filename);
    state.SetItemsProcessed(state.iterations());
}

static void Bm_function_Getpgrp(benchmark::State &state)
{
    int pgid = -1;
    for (auto _ : state) {
        pgid = getpgrp();
        if (pgid == -1) {
            perror("getpgrp proc");
        }
        benchmark::DoNotOptimize(pgid);
    }
}

// Deletes a file or directory under the specified path
// delete file
static void Bm_function_Unlinkat_ZeroFlag(benchmark::State &state)
{
    const char* path = "/data/data/testUnlinkatZero.txt";
    int fd = -1;
    for (auto _ : state) {
        state.PauseTiming();
        fd = open(path, O_RDWR | O_CREAT, OPEN_MODE);
        if (fd == -1) {
            perror("open fstatUnlinkat zero");
        }
        state.ResumeTiming();
        if (unlinkat(fd, path, 0) != 0) {
            perror("unlinkat zero proc");
        }
        state.PauseTiming();
        close(fd);
        state.ResumeTiming();
    }
}

// delete directory
static void Bm_function_Unlinkat_AT_REMOVEDIR(benchmark::State &state)
{
    const char* path = "/data/data/testUnlinkatAtRemoveDir";
    for (auto _ : state) {
        state.PauseTiming();
        if (mkdir(path, S_IRWXU | S_IRWXG | S_IXGRP | S_IROTH | S_IXOTH) == -1) {
            perror("mkdir unlinkat removedir");
        }
        state.ResumeTiming();
        if (unlinkat(AT_FDCWD, path, AT_REMOVEDIR) != 0) {
            perror("unlinkat removedir proc");
        }
    }
}

static void Bm_function_Execlp_ls(benchmark::State &state)
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork execlp ls");
    }
    for (auto _ : state) {
        if (pid == 0) {
            benchmark::DoNotOptimize(execlp("/bin/ls", "ls", "/tmp", nullptr));
        }
    }
}

static void Bm_function_Sysconf(benchmark::State &state)
{
    for (auto _ : state) {
        benchmark::DoNotOptimize(sysconf(_SC_PAGESIZE));
    }
}

static void Bm_function_Prctl(benchmark::State &state)
{
    size_t pagesize = sysconf(_SC_PAGE_SIZE);
    void *addr = mmap(nullptr, pagesize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == nullptr) {
        perror("mmap error");
    }
    static char addrName[] = "prctl_test";
    for (auto _ : state) {
        benchmark::DoNotOptimize(prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, addr, pagesize, addrName));
    }
    munmap(addr, pagesize);
}

MUSL_BENCHMARK(Bm_function_Getpid);
MUSL_BENCHMARK(Bm_function_Geteuid);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Pwrite64, "COMMON_ARGS");
MUSL_BENCHMARK(Bm_function_Readlink);
MUSL_BENCHMARK(Bm_function_Readlinkat);
MUSL_BENCHMARK(Bm_function_Readlinkat_chk);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Pread64, "COMMON_ARGS");
MUSL_BENCHMARK(Bm_function_Close);
MUSL_BENCHMARK(Bm_function_Usleep);
MUSL_BENCHMARK(Bm_function_Getuid);
MUSL_BENCHMARK(Bm_function_Getegid);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Read, "COMMON_ARGS");
MUSL_BENCHMARK(Bm_function_Access_exist);
MUSL_BENCHMARK(Bm_function_Access_read);
MUSL_BENCHMARK(Bm_function_Access_write);
MUSL_BENCHMARK(Bm_function_Access_execute);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Writev, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Uname);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Write, "BENCHMARK_8");
MUSL_BENCHMARK(Bm_function_Lseek);
MUSL_BENCHMARK(Bm_function_Dup);
MUSL_BENCHMARK_WITH_APPLY(Bm_function_Pipe2, PrepareArgsPipe2);
MUSL_BENCHMARK(Bm_function_Getopt);
MUSL_BENCHMARK(Bm_function_Fsync);
MUSL_BENCHMARK(Bm_function_Fdatasync);
MUSL_BENCHMARK(Bm_function_Ftruncate);
MUSL_BENCHMARK(Bm_function_Unlink);
MUSL_BENCHMARK(Bm_function_Faccessat_Normal);
MUSL_BENCHMARK(Bm_function_Faccessat_Nofollow);
MUSL_BENCHMARK_WITH_ARG(Bm_function_Readv, "ALIGNED_TWOBUF");
MUSL_BENCHMARK(Bm_function_Chown);
MUSL_BENCHMARK(Bm_function_Getpgrp);
MUSL_BENCHMARK(Bm_function_Unlinkat_ZeroFlag);
MUSL_BENCHMARK(Bm_function_Unlinkat_AT_REMOVEDIR);
MUSL_BENCHMARK(Bm_function_Execlp_ls);
MUSL_BENCHMARK(Bm_function_Sysconf);
MUSL_BENCHMARK(Bm_function_Prctl);

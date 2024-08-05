/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sigchain.h>
#include "fortify_test.h"
#include "test.h"

/**
 * @tc.name     : getcwd
 * @tc.desc     : Exception scenarios for getcwd
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_001(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };

    sigaction(SIGABRT, &sigabrt, NULL);
    const int bufferSize = 1;
    char buf[bufferSize];
    size_t n = atoi("2");
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            getcwd(buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    return;
}

/**
 * @tc.name     : pread
 * @tc.desc     : Exception scenarios for pread
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_002(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const int err = -1;
    int fd = open("/proc/version", O_RDWR | O_CREAT, 0777);
    TEST(fd != err);

    int status;
    const int bufferSize = 2;
    char buf[bufferSize];
    size_t n = atoi("4");
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            pread(fd, buf, n, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : pread
 * @tc.desc     : Exception scenarios for pread
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_003(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const int err = -1;
    int fd = open("/proc/version", O_RDWR | O_CREAT, 0777);
    TEST(fd != err);

    int status;
    const int bufferSize = 2;
    char buf[bufferSize];
    size_t n = atoi("4") + SSIZE_MAX;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            pread(fd, buf, n, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}


/**
 * @tc.name     : pread64
 * @tc.desc     : Exception scenarios for pread64
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_004(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const int err = -1;
    int fd = open("/proc/version", O_RDWR | O_CREAT, 0777);
    TEST(fd != err);
    int status;

    const int bufferSize = 2;
    char buf[bufferSize];
    size_t n = atoi("4") + SSIZE_MAX;;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            pread64(fd, buf, n, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : pread64
 * @tc.desc     : Exception scenarios for pread64
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_005(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const int err = -1;
    int fd = open("/proc/version", O_RDWR | O_CREAT, 0777);
    TEST(fd != err);
    int status;

    const int bufferSize = 2;
    char buf[bufferSize];
    size_t n = atoi("4");
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            pread64(fd, buf, n, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : pwrite64
 * @tc.desc     : Exception scenarios for pwrite64
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_006(void)
{
    char fileName[] = "./unistd_dynamic_chk_003.txt";
    int fd = open(fileName, O_RDWR | O_CREAT, 0777);
    TEST(fd != -1);

    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char buf[] = "hello!";
    size_t n = strlen(buf) + 2;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            pwrite64(fd, buf, n, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : pwrite64
 * @tc.desc     : Exception scenarios for pwrite64
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_007(void)
{
    char fileName[] = "./unistd_dynamic_chk_003.txt";
    int fd = open(fileName, O_RDWR | O_CREAT, 0777);
    TEST(fd != -1);

    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char buf[] = "hello!";
    size_t n = strlen(buf) + SSIZE_MAX;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            pwrite64(fd, buf, n, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}


/**
 * @tc.name     : pwrite
 * @tc.desc     : Exception scenarios for pwrite
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_008(void)
{
    const int err = -1;
    char fileName[] = "./unistd_dynamic_chk_003.txt";
    int fd = open(fileName, O_RDWR | O_CREAT, 0777);
    TEST(fd != err);

    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char buf[] = "hello!";
    size_t n = strlen(buf) + 2;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            pwrite(fd, buf, n, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : pwrite
 * @tc.desc     : Exception scenarios for pwrite
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_009(void)
{
    const int err = -1;
    char fileName[] = "./unistd_dynamic_chk_003.txt";
    int fd = open(fileName, O_RDWR | O_CREAT, 0777);
    TEST(fd != err);

    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char buf[] = "hello!";
    size_t n = strlen(buf) + SSIZE_MAX;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            pwrite(fd, buf, n, 0);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : read
 * @tc.desc     : Exception scenarios for read
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_010(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int fd = open("/proc/version", O_RDWR | O_CREAT, 0777);
    int status;

    const int bufferSize = 2;
    char buf[bufferSize];
    size_t n = atoi("4");
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            read(fd, buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : read
 * @tc.desc     : Exception scenarios for read
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_011(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int fd = open("/proc/version", O_RDWR | O_CREAT, 0777);
    int status;

    const int bufferSize = 2;
    char buf[bufferSize];
    size_t n = atoi("4") + SSIZE_MAX;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            read(fd, buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : write
 * @tc.desc     : Exception scenarios for write
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_012(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const int err = -1;
    char fileName[] = "./unistd_dynamic_chk_005.txt";
    int fd = open(fileName, O_RDWR | O_CREAT, 0777);
    TEST(fd != err);

    char buf[] = "hello!";
    size_t n = strlen(buf) + 2;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            write(fd, buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : write
 * @tc.desc     : Exception scenarios for write
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_013(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const int err = -1;
    char fileName[] = "./unistd_dynamic_chk_005.txt";
    int fd = open(fileName, O_RDWR | O_CREAT, 0777);
    TEST(fd != err);

    char buf[] = "hello!";
    size_t n = strlen(buf) + SSIZE_MAX;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            write(fd, buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    close(fd);
    return;
}

/**
 * @tc.name     : readlink
 * @tc.desc     : Exception scenarios for readlink
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_014(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char path[] = "/dev/null";

    const int bufferSize = 1;
    char buf[bufferSize];
    size_t n = atoi("2");
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            readlink(path, buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    return;
}

/**
 * @tc.name     : readlink
 * @tc.desc     : Exception scenarios for readlink
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_015(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char path[] = "/dev/null";

    const int bufferSize = 1;
    char buf[bufferSize];
    size_t n = atoi("2") + SSIZE_MAX;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            readlink(path, buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    return;
}
/**
 * @tc.name     : readlinkat
 * @tc.desc     : Exception scenarios for readlinkat
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_016(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char path[] = "/dev/null";

    const int bufferSize = 1;
    char buf[bufferSize];

    size_t n = atoi("2");
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            readlinkat(AT_FDCWD, path, buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    return;
}

/**
 * @tc.name     : readlinkat
 * @tc.desc     : Exception scenarios for readlinkat
 * @tc.level    : Level 2
 */
static void unistd_dynamic_chk_017(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char path[] = "/dev/null";

    const int bufferSize = 1;
    char buf[bufferSize];

    size_t n = atoi("1") + SSIZE_MAX;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            readlinkat(AT_FDCWD, path, buf, n);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    return;
}
int main(int argc, char *argv[])
{
    remove_all_special_handler(SIGABRT);
    unistd_dynamic_chk_001();
    unistd_dynamic_chk_002();
    unistd_dynamic_chk_003();
    unistd_dynamic_chk_004();
    unistd_dynamic_chk_005();
    unistd_dynamic_chk_006();
    unistd_dynamic_chk_007();
    unistd_dynamic_chk_008();
    unistd_dynamic_chk_009();
    unistd_dynamic_chk_010();
    unistd_dynamic_chk_011();
    unistd_dynamic_chk_012();
    unistd_dynamic_chk_013();
    unistd_dynamic_chk_014();
    unistd_dynamic_chk_015();
    unistd_dynamic_chk_016();
    unistd_dynamic_chk_017();
    return t_status;
}
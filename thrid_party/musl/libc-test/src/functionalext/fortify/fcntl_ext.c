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
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sigchain.h>
#include "fortify_test.h"
#include "functionalext.h"
#include "test.h"

#define FILE_MODE_ALL (0777)

#ifdef open64
#undef open64
#endif

#ifdef openat64
#undef openat64
#endif

/**
 * @tc.name     : open_0010
 * @tc.desc     : test open normal condition
 * @tc.level    : Level 0
 */
static void open_0010(void)
{
    int fd = open("/proc/version", O_RDWR | O_CREAT, FILE_MODE_ALL);
    TEST(fd != -1);
    close(fd);

    return;
}

/**
 * @tc.name     : open_0020
 * @tc.desc     : test open O_CREAT without mode
 * @tc.level    : Level 2
 */
static void open_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int flags = O_CREAT;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            open("/proc/version", flags);
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
 * @tc.name     : open_0030
 * @tc.desc     : test open O_TMPFILE without mode
 * @tc.level    : Level 2
 */
static void open_0030(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int flags = O_TMPFILE;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            open("/proc/version", flags);
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
 * @tc.name     : open_0040
 * @tc.desc     : test open only O_RDWR
 * @tc.level    : Level 1
 */
static void open_0040(void)
{
    int fd = open("/proc/version", O_RDWR);
    EXPECT_NE(open_0040, fd, -1);
    close(fd);

    return;
}

/**
 * @tc.name     : openat_0010
 * @tc.desc     : test openat normal condition
 * @tc.level    : Level 0
 */
static void openat_0010(void)
{
    int fd = openat(AT_FDCWD, "/proc/version", O_RDWR | O_CREAT, FILE_MODE_ALL);
    TEST(fd != -1);
    close(fd);

    return;
}

/**
 * @tc.name     : openat_0020
 * @tc.desc     : test openat O_CREAT without mode
 * @tc.level    : Level 2
 */
static void openat_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int flags = O_CREAT;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            openat(AT_FDCWD, "/proc/version", flags);
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
 * @tc.name     : openat_0030
 * @tc.desc     : test openat O_TMPFILE without mode
 * @tc.level    : Level 2
 */
static void openat_0030(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int flags = O_TMPFILE;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            openat(AT_FDCWD, "/proc/version", flags);
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
 * @tc.name     : openat_0040
 * @tc.desc     : test openat only O_RDWR
 * @tc.level    : Level 1
 */
static void openat_0040(void)
{
    int fd = openat(AT_FDCWD, "/proc/version", O_RDWR);
    EXPECT_NE(openat_0040, fd, -1);
    close(fd);

    return;
}

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
/**
 * @tc.name     : open64_0010
 * @tc.desc     : test open64 normal condition
 * @tc.level    : Level 0
 */
static void open64_0010(void)
{
    int fd = open64("/proc/version", O_RDWR | O_CREAT, FILE_MODE_ALL);
    TEST(fd != -1);
    close(fd);

    return;
}

/**
 * @tc.name     : open64_0020
 * @tc.desc     : test open64 O_CREAT without mode
 * @tc.level    : Level 2
 */
static void open64_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int flags = O_CREAT;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            open64("/proc/version", flags);
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
 * @tc.name     : open64_0030
 * @tc.desc     : test open64 O_TMPFILE without mode
 * @tc.level    : Level 2
 */
static void open64_0030(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int flags = O_TMPFILE;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            open64("/proc/version", flags);
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
 * @tc.name     : open64_0040
 * @tc.desc     : test open64 only O_RDWR
 * @tc.level    : Level 1
 */
static void open64_0040(void)
{
    int fd = open64("/proc/version", O_RDWR);
    EXPECT_NE(open64_0040, fd, -1);
    close(fd);

    return;
}

/**
 * @tc.name     : openat64_0010
 * @tc.desc     : test openat64 normal condition
 * @tc.level    : Level 0
 */
static void openat64_0010(void)
{
    int fd = openat64(AT_FDCWD, "/proc/version", O_RDWR | O_CREAT, FILE_MODE_ALL);
    TEST(fd != -1);
    close(fd);

    return;
}

/**
 * @tc.name     : openat64_0020
 * @tc.desc     : test openat64 O_CREAT without mode
 * @tc.level    : Level 2
 */
static void openat64_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int flags = O_CREAT;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            openat64(AT_FDCWD, "/proc/version", flags);
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
 * @tc.name     : openat64_0030
 * @tc.desc     : test openat64 O_TMPFILE without mode
 * @tc.level    : Level 2
 */
static void openat64_0030(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int flags = O_TMPFILE;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            openat64(AT_FDCWD, "/proc/version", flags);
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
 * @tc.name     : openat64_0040
 * @tc.desc     : test openat64 only O_RDWR
 * @tc.level    : Level 1
 */
static void openat64_0040(void)
{
    int fd = openat64(AT_FDCWD, "/proc/version", O_RDWR);
    EXPECT_NE(openat64_0040, fd, -1);
    close(fd);

    return;
}
#endif

int main(int argc, char *argv[]) {
    remove_all_special_handler(SIGABRT);
    open_0010();
    open_0020();
    open_0030();
    open_0040();
    openat_0010();
    openat_0020();
    openat_0030();
    openat_0040();
#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
    open64_0010();
    open64_0020();
    open64_0030();
    open64_0040();
    openat64_0010();
    openat64_0020();
    openat64_0030();
    openat64_0040();
#endif
    return t_status;
}
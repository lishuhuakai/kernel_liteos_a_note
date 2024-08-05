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
#include <limits.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sigchain.h>
#include "fortify_test.h"
#include "functionalext.h"
#include "test.h"
#include "../../../../include/fortify/linux/fortify.h"

#define SIZE_1 1
#define SIZE_5 5
#define SIZE_7 7
#define SIZE_10 10
#define SIZE_11 11
#define SIZE_15 15
#define SIZE_20 20
#define EQ_0 '0'
#define EQ_1 '1'
#define EQ_5 '5'
#define EQ_8 '8'
#define EQ_9 '9'
#define EQ_10 "10"
#define EQ_11 "11"
#define STRLEN_4 "1234"
#define STRLEN_5 "01234"
#define STRLEN_9 "123456789"
#define STRLEN_10 "0123456789"
#define STRLEN_14 "01234567890123"

/**
 * @tc.name     : test_strcat_0010
 * @tc.desc     : After adding fortify, test the normal strcat of the function.
 * @tc.level    : Level 0
 */
static void test_strcat_0010()
{
    char src[SIZE_15];
    strcpy(src, STRLEN_10);
    char dst[SIZE_20];
    memset(dst, 0, SIZE_20);
    strcat(dst, src);
    TEST(dst[0] == EQ_0);
}

/**
 * @tc.name     : test_strcat_0020
 * @tc.desc     : Ability to test the strcat Fortify runtime
 * @tc.level    : Level 2
 */
static void test_strcat_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char src[SIZE_15];
    strcpy(src, STRLEN_10);
    char dst[SIZE_5];
    memset(dst, 0, SIZE_5);
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strcat(dst, src);
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
 * @tc.name     : test_strcat_0010
 * @tc.desc     : After adding fortify, test the normal strcat of the function.
 * @tc.level    : Level 0
 */
static void test_strncat_0010()
{
    char src[SIZE_15];
    strcpy(src, STRLEN_10);
    char dst[SIZE_20];
    memset(dst, 0, SIZE_20);
    strncat(dst, src, strlen(src));
    TEST(dst[0] == EQ_0);
}

/**
 * @tc.name     : test_strcat_0020
 * @tc.desc     : Ability to test the strcat Fortify runtime
 * @tc.level    : Level 2
 */
static void test_strncat_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char src[SIZE_15];
    strcpy(src, STRLEN_10);
    char dst[SIZE_5];
    memset(dst, 0, SIZE_5);
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strncat(dst, src, strlen(src));
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
 * @tc.name     : test_stpcpy_0010
 * @tc.desc     : After adding fortify, test the normal stpcpy of the function.
 * @tc.level    : Level 0
 */
static void test_stpcpy_0010()
{
    char *src = "abcdefg";
    char dst[SIZE_15];
    stpcpy(dst, src);
    TEST(dst[0] == 'a');
}

/**
 * @tc.name     : test_stpcpy_0020
 * @tc.desc     : Ability to test the stpcpy Fortify runtime
 * @tc.level    : Level 2
 */
static void test_stpcpy_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char *src = STRLEN_9;
    char dst[SIZE_5];
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            stpcpy(dst, src);
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
 * @tc.name     : test_stpncpy_0010
 * @tc.desc     : After adding fortify, test the normal strcpy of the function.
 * @tc.level    : Level 0
 */
static void test_stpncpy_0010()
{
    char src[SIZE_10];
    char dst[SIZE_15];
    strcpy(src, STRLEN_4);
    size_t n = strlen(src);
    stpncpy(dst, src, n);
    TEST(dst[0] == EQ_1);
}

/**
 * @tc.name     : test_stpncpy_0020
 * @tc.desc     : Ability to test the strcpy Fortify runtime
 * @tc.level    : Level 2
 */
static void test_stpncpy_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char src[SIZE_15];
    char dst[SIZE_5];
    strcpy(src, STRLEN_10);
    size_t n = strlen(src);
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            stpncpy(dst, src, n);
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
 * @tc.name     : test_strncpy_0010
 * @tc.desc     : After adding fortify, test the normal strncpy of the function.
 * @tc.level    : Level 0
 */
static void test_strncpy_0010()
{
    char src[SIZE_10] = STRLEN_10;
    char dst[SIZE_15];
    strncpy(dst, src, SIZE_5);
    TEST(src != dst);
}

/**
 * @tc.name     : test_strncpy_0020
 * @tc.desc     : Ability to test the strncpy Fortify runtime
 * @tc.level    : Level 2
 */
static void test_strncpy_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char src[SIZE_15];
    char dst[SIZE_10];
    strcpy(src, STRLEN_14);
    size_t n = strlen(src);

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strncpy(dst, src, n);
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
 * @tc.name     : test_memchr_0010
 * @tc.desc     : After adding fortify, test the normal memchr of the function.
 * @tc.level    : Level 0
 */
static void test_memchr_0010()
{
    const char str[] = STRLEN_9;
    const char ch = EQ_9;
    char *ret = (char*)memchr(str, ch, strlen(str));
    TEST(*ret == EQ_9);
}

/**
 * @tc.name     : test_memchr_0020
 * @tc.desc     : Ability to test the memchr Fortify runtime
 * @tc.level    : Level 2
 */
static void test_memchr_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const char s[SIZE_10] = STRLEN_9;
    const char c = EQ_5;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            memchr(s, c, SIZE_20);
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
 * @tc.name     : test_memrchr_0010
 * @tc.desc     : After adding fortify, test the normal memrchr of the function.
 * @tc.level    : Level 0
 */
static void test_memrchr_0010()
{
    const char str[] = STRLEN_9;
    const char ch = EQ_9;
    char *ret = (char*)memrchr(str, ch, strlen(str));
    TEST(*ret == EQ_9);
}

/**
 * @tc.name     : test_memrchr_0020
 * @tc.desc     : Ability to test the memrchr Fortify runtime
 * @tc.level    : Level 2
 */
static void test_memrchr_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const char s[SIZE_10] = STRLEN_9;
    const char c = EQ_5;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            memrchr(s, c, SIZE_20);
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
 * @tc.name     : test_strchr_0010
 * @tc.desc     : After adding fortify, test the normal strchr of the function.
 * @tc.level    : Level 0
 */
static void test_strchr_0010()
{
    const char str[] = STRLEN_9;
    const char ch = EQ_9;
    char *ret = strchr(str, ch);
    TEST(*ret == EQ_9);
}

/**
 * @tc.name     : test_strchr_0020
 * @tc.desc     : Ability to test the strchr Fortify runtime
 * @tc.level    : Level 2
 */
static void test_strchr_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char str[0];
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strchr(str, 'a');
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
 * @tc.name     : test_strrchr_0010
 * @tc.desc     : After adding fortify, test the normal strrchr of the function.
 * @tc.level    : Level 0
 */
static void test_strrchr_0010()
{
    int len;
    const char str[] = STRLEN_9;
    const char ch = EQ_9;
    char *ret = strrchr(str, ch);
    TEST(*ret == EQ_9);
}

/**
 * @tc.name     : test_strrchr_0020
 * @tc.desc     : Ability to test the strrchr Fortify runtime
 * @tc.level    : Level 2
 */
static void test_strrchr_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char str[0];
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strrchr(str, 'a');
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
 * @tc.name     : test_strlcat_0010
 * @tc.desc     : After adding fortify, test the normal strlcat of the function.
 * @tc.level    : Level 0
 */
#if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
static void test_strlcat_0010()
{
    char dst[SIZE_10];
    char src[SIZE_5] = STRLEN_4;
    memset(dst, 0, SIZE_10);
    strlcat(dst, src, strlen(src));
    TEST(dst[0] == EQ_1);
}

/**
 * @tc.name     : test_strlcat_0020
 * @tc.desc     : Ability to test the strlcat Fortify runtime
 * @tc.level    : Level 2
 */
static void test_strlcat_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char dst[SIZE_5] = STRLEN_4;
    char src[SIZE_10] = STRLEN_9;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strlcat(dst, src, strlen(src));
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
 * @tc.name     : test_strlcpy_0010
 * @tc.desc     : After adding fortify, test the normal strcpy of the function.
 * @tc.level    : Level 0
 */
static void test_strlcpy_0010()
{
    char src[SIZE_10];
    char dst[SIZE_15];
    memset(dst, 0, SIZE_15);
    strcpy(src, STRLEN_4);
    size_t n = strlen(src);
    strlcpy(dst, src, n);
    TEST(dst[0] == EQ_1);
}

/**
 * @tc.name     : test_strlcpy_0020
 * @tc.desc     : Ability to test the strcpy Fortify runtime
 * @tc.level    : Level 2
 */
static void test_strlcpy_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char src[SIZE_15];
    char dst[SIZE_10];
    strcpy(src, STRLEN_14);
    size_t n = strlen(src);
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strlcpy(dst, src, n);
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
#endif

/**
 * @tc.name     : test_mempcpy_0010
 * @tc.desc     : After adding fortify, test the normal mempcpy of the function.
 * @tc.level    : Level 0
 */
static void test_mempcpy_0010()
{
    char dst[SIZE_20];
    char src[SIZE_15] = STRLEN_10;
    mempcpy(dst, src, strlen(src));
    TEST(dst[0] == EQ_0);
}

/**
 * @tc.name     : test_mempcpy_0020
 * @tc.desc     : Ability to test the mempcpy Fortify runtime
 * @tc.level    : Level 2
 */
static void test_mempcpy_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char dst[SIZE_5] = STRLEN_4;
    char src[SIZE_20] = STRLEN_10;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            mempcpy(dst, src, strlen(src));
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
 * @tc.name     : test_mempcpy_0030
 * @tc.desc     : Ability to test the mempcpy Fortify runtime
 * @tc.level    : Level 2
 */
static void test_mempcpy_0030()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char dst[SIZE_5] = STRLEN_4;
    char src[SIZE_20] = STRLEN_10;
    size_t n = strlen(src) + SSIZE_MAX;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            mempcpy(dst, src, n);
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
 * @tc.name     : test_strcpy_0010
 * @tc.desc     : After adding fortify, test the normal strcpy of the function.
 * @tc.level    : Level 0
 */
static void test_strcpy_0010()
{
    char dst[SIZE_10];
    char src[SIZE_10] = {STRLEN_4};
    strcpy(dst, src);
    TEST(dst[0] == EQ_1);
}

/**
 * @tc.name     : test_strcpy_0020
 * @tc.desc     : Ability to test the strcpy Fortify runtime
 * @tc.level    : Level 2
 */
static void test_strcpy_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char dst[2], src[SIZE_5] = {STRLEN_4};
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strcpy(dst, src);
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
 * @tc.name     : test_memmove_0010
 * @tc.desc     : After adding fortify, test the normal memmove of the function.
 * @tc.level    : Level 0
 */
static void test_memmove_0010()
{
    char s[] = STRLEN_9;
    memmove(s, s+SIZE_7, strlen(s)+1-SIZE_7);
    TEST(s[0] == EQ_8);
}

/**
 * @tc.name     : test_memmove_0020
 * @tc.desc     : Ability to test the memmove Fortify runtime
 * @tc.level    : Level 2
 */
static void test_memmove_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char dst[SIZE_20];
    strcpy(dst, STRLEN_10);
    size_t n = atoi(EQ_10);
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            memmove(dst + SIZE_11, dst, n);
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
 * @tc.name     : test_memcpy_0010
 * @tc.desc     : After adding fortify, test the normal memcpy of the function.
 * @tc.level    : Level 0
 */
static void test_memcpy_0010()
{
    char dst[SIZE_15];
    memcpy(dst, STRLEN_10, SIZE_10);
    TEST(dst[0] == EQ_0);
}

/**
 * @tc.name     : test_memcpy_0020
 * @tc.desc     : Ability to test the memcpy Fortify runtime
 * @tc.level    : Level 2
 */
static void test_memcpy_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);


    int status;
    char dst[SIZE_10];
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            memcpy(dst, STRLEN_14, SIZE_15);
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
 * @tc.name     : test_memset_0010
 * @tc.desc     : After adding fortify, test the normal memset of the function.
 * @tc.level    : Level 0
 */
static void test_memset_0010()
{
    char src[SIZE_5] = STRLEN_5;
    char dst[SIZE_5] = STRLEN_5;
    memset(dst, 0, SIZE_5);
    TEST(dst != src);
}

/**
 * @tc.name     : test_memset_0020
 * @tc.desc     : Ability to test the memset Fortify runtime
 * @tc.level    : Level 2
 */
static void test_memset_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char buf[SIZE_10];
    size_t n = atoi(EQ_11);
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            memset(buf, 0, n);
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
 * @tc.name     : test_strlen_0010
 * @tc.desc     : Ability to test the strlen normal condition
 * @tc.level    : Level 0
 */
static void test_strlen_0010()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char buf[SIZE_10];
    memcpy(buf, STRLEN_10, sizeof(buf));
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strlen(buf);
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
 * @tc.name     : test_strlen_0020
 * @tc.desc     : Ability to test the strlen with NULL
 * @tc.level    : Level 2
 */
static void test_strlen_0020()
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            strlen(NULL);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            EXPECT_EQ(test_strlen_0020, WIFEXITED(status), 0);
            kill(pid, SIGCONT);
            break;
    }
    return;
}

int main(int argc, char *argv[]) {
    remove_all_special_handler(SIGABRT);
    test_strcat_0010();
    test_strcat_0020();
    test_strncat_0010();
    test_strncat_0020();
    test_strchr_0010();
    test_strchr_0020();
    test_strncpy_0010();
    test_strncpy_0020();
    test_stpcpy_0010();
    test_stpcpy_0020();
    test_stpncpy_0010();
    test_stpncpy_0020();
    test_memchr_0010();
    test_memchr_0020();
    test_strrchr_0010();
    test_strrchr_0020();
    test_strcpy_0010();
    test_strcpy_0020();
    test_memmove_0010();
    test_memmove_0020();
    test_memset_0010();
    test_memset_0020();
    test_memcpy_0010();
    test_memcpy_0020();
    test_strlen_0010();
    test_strlen_0020();
    
    #ifdef _GNU_SOURCE
    test_mempcpy_0010();
    test_mempcpy_0020();
    test_mempcpy_0030();
    test_memrchr_0010();
    test_memrchr_0020();
    #endif

    #if defined(_BSD_SOURCE) || defined(_GNU_SOURCE)
    test_strlcat_0010();
    test_strlcat_0020();
    test_strlcpy_0010();
    test_strlcpy_0020();
    #endif
    return t_status;
}
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

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sigchain.h>
#include "fortify_test.h"
#include "test.h"

#define EXPECT_EQ(a, b) \
    do { \
        if ((a) != (b)) \
            t_error("failed!\n"); \
    } while (0)

#define EXPECT_STREQ(a, b) \
    do { \
        size_t minlen = strlen(a) >= strlen(b) ? strlen(b) : strlen(a); \
        if (strncmp(a, b, minlen) != 0) \
            t_error("failed\n"); \
    } while (0)

#define EXPECT_TRUE(c) \
    do { \
        if (!(c)) \
            t_error("failed!\n"); \
    } while (0)

/**
 * @tc.name     : fread
 * @tc.desc     : normal use
 * @tc.level    : Level 0
 */
static void stdio_dynamic_chk_001(void)
{
    char hello_world[] = "hello world!";
    FILE *fp = fmemopen(hello_world, sizeof(hello_world), "r");
    EXPECT_TRUE(fp);

    const int bufferSize = 14;
    char buf[bufferSize]; // > sizeof(hello_world)
    EXPECT_EQ(1u, fread(buf, sizeof(hello_world), 1, fp));
    EXPECT_STREQ(hello_world, buf);

    fclose(fp);
    return;
}

/**
 * @tc.name     : fread
 * @tc.desc     : normal use
 * @tc.level    : Level 0
 */
static void stdio_dynamic_chk_002(void)
{
    FILE *fp = fopen("/dev/zero", "r");
    EXPECT_TRUE(fp);

    setvbuf(fp, NULL, _IONBF, 0);

    const int bufferSize = 65*1024;
    char buf[bufferSize];
    memset(buf, 0xff, sizeof(buf));

    size_t read_size = 64*1024;
    size_t count_size = 1024;
    for (size_t i = 0; i < count_size; ++i) {
        EXPECT_EQ(1u, fread(buf, read_size, 1, fp));
    }

    // The first 64*1024 should be assigned
    for (size_t i = 0; i < read_size; ++i) {
        EXPECT_EQ('\0', buf[i]);
    }

    // What's left is its original data
    for (size_t i = read_size; i < bufferSize; ++i) {
        EXPECT_EQ('\xff', buf[i]);
    }
    return;
}

/**
 * @tc.name     : fread
 * @tc.desc     : Exceed the buffer range and reach dynamic monitoring conditions
 * @tc.level    : Level 2
 */
static void stdio_dynamic_chk_003(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    const int bufferSize = 1;
    char buf[bufferSize];
    size_t ct = atoi("2");
    FILE* fp = fopen("/dev/null", "r");

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            fread(buf, 1, ct, fp);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    fclose(fp);
    return;
}

/**
 * @tc.name     : fwrite
 * @tc.desc     : Exceed the buffer range and reach dynamic monitoring conditions
 * @tc.level    : Level 2
 */
static void stdio_dynamic_chk_004(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    size_t ct = atoi("2");
    FILE* fp = fopen("/dev/null", "w");

    const int bufferSize = 1;
    char buf[bufferSize];

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            fwrite(buf, 1, ct, fp);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    fclose(fp);
    return;
}

/**
 * @tc.name     : fgets
 * @tc.desc     : Normal function
 * @tc.level    : Level 0
 */
static void stdio_dynamic_chk_005(void)
{
    char hello_world[] = "hello world!";
    FILE *fp = fmemopen(hello_world, sizeof(hello_world), "r");
    EXPECT_TRUE(fp);

    const int bufferSize = 16;
    char buf[bufferSize];
    char *get = fgets(buf, sizeof(buf), fp);
    EXPECT_TRUE(get != NULL);
    EXPECT_TRUE(strcmp(hello_world, get) == 0);
    fclose(fp);
    return;
}

/**
 * @tc.name     : fgets
 * @tc.desc     : Get newline and end position as normal
 * @tc.level    : Level 0
 */
static void stdio_dynamic_chk_006(void)
{
    char hello_world[] = "hello world!\nhello boy!\0";
    FILE *fp = fmemopen(hello_world, sizeof(hello_world), "r");
    EXPECT_TRUE(fp);

    const int bufferSize = 16;
    char buf[bufferSize];
    char *get1 = fgets(buf, sizeof("hello"), fp);
    EXPECT_TRUE(get1 != NULL);
    EXPECT_TRUE(strcmp("hello", get1) == 0);

    memset(buf,0x00,sizeof(buf));
    char *get2 = fgets(buf, sizeof(buf), fp);
    EXPECT_TRUE(get2 != NULL);

    memset(buf,0x00,sizeof(buf));
    char *get3 = fgets(buf, sizeof(buf), fp);
    EXPECT_TRUE(get3 != NULL);
    EXPECT_TRUE(strcmp("hello boy!", get3) == 0);
    fclose(fp);
    return;
}

/**
 * @tc.name     : fgets
 * @tc.desc     : The size of reads is greater than the capacity of buf
 * @tc.level    : Level 2
 */
static void stdio_dynamic_chk_007(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    char hello_world[] = "hello world!";
    FILE *fp = fmemopen(hello_world, sizeof(hello_world), "r");

    const int bufferSize = 16;
    char buf[bufferSize];
    size_t n = atoi("18");
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            fgets(buf, n, fp);
            exit(0);
        default:
            waitpid(pid, &status, WUNTRACED);
            TEST(WIFEXITED(status) == 0);
            TEST(WIFSTOPPED(status) == 1);
            TEST(WSTOPSIG(status) == SIGSTOP);
            kill(pid, SIGCONT);
            break;
    }
    fclose(fp);
    return;
}

/**
 * @tc.name     : sprintf
 * @tc.desc     : Normal call test
 * @tc.level    : Level 0
 */
static void stdio_dynamic_chk_008(void)
{
    char buf[] = "world";
    sprintf(buf, "hello");
    EXPECT_TRUE(strcmp(buf, "hello") == 0);
    return;
}

/**
 * @tc.name     : sprintf
 * @tc.desc     : Normal call test
 * @tc.level    : Level 0
 */
static void stdio_dynamic_chk_009(void)
{
    const int bufferSize = 20;
    char buf[bufferSize];
    sprintf(buf, "hello : %s", "world!");

    char value[] = "hello : world!";
    EXPECT_TRUE(strcmp(buf, value) == 0);
    return;
}


/**
 * @tc.name     : sprintf
 * @tc.desc     : sprintf beyond capacity
 * @tc.level    : Level 2
 */
static void stdio_dynamic_chk_010(void)
{
    const int bufferSize = 6;
    char buf[bufferSize];

    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            sprintf(buf, "hello : %s", "world!");
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
 * @tc.name     : snprintf
 * @tc.desc     : snprintf beyond capacity
 * @tc.level    : Level 2
 */

static void stdio_dynamic_chk_011(void)
{
    const int bufferSize = 6;
    char buf[bufferSize];

    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int printSize = 10;
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0: // 10 > sizeof buf
            snprintf(buf, printSize, "hello : %s", "world!");
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


static int vsnprintf_test(const char* format, ...)
{
    const int bufferSize = 6;
    char buf[bufferSize];
    int printSize = 10;
    va_list va;
    va_start(va, format);
    int result = vsnprintf(buf, printSize, format, va);
    va_end(va);
    return result;
}

static int vsprintf_test(const char* format, ...)
{
    const int bufferSize = 6;
    char buf[bufferSize];
    va_list va;
    va_start(va, format);
    int result = vsprintf(buf, format, va);
    va_end(va);
    return result;
}

/**
 * @tc.name     : vsnprintf
 * @tc.desc     : vsnprintf beyond capacity
 * @tc.level    : Level 2
 */
static void stdio_dynamic_chk_012(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            vsnprintf_test("hello : %s", "world!");
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
 * @tc.name     : vsprsintf
 * @tc.desc     : vsprintf beyond capacity
 * @tc.level    : Level 2
 */
static void stdio_dynamic_chk_013(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %d\n", __LINE__);
            break;
        case 0:
            vsprintf_test("%s", "0123456789");
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

int main()
{
    remove_all_special_handler(SIGABRT);
    stdio_dynamic_chk_001();
    stdio_dynamic_chk_002();
    stdio_dynamic_chk_003();
    stdio_dynamic_chk_004();
    stdio_dynamic_chk_005();
    stdio_dynamic_chk_006();
    stdio_dynamic_chk_007();
    stdio_dynamic_chk_008();
    stdio_dynamic_chk_009();
    stdio_dynamic_chk_010();
    stdio_dynamic_chk_011();
    stdio_dynamic_chk_012();
    stdio_dynamic_chk_013();

    return t_status;
}
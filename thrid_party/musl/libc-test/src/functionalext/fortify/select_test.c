/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sigchain.h>
#include "fortify_test.h"
#include "functionalext.h"
#include "test.h"

/**
 * @tc.name     : fd_set_0010
 * @tc.desc     : test FD_SET with normal fd
 * @tc.level    : Level 1
 */
static void fd_set_0010(void)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    int sockfd = 0;
    FD_SET(sockfd, &readfds);
    EXPECT_EQ(fd_set_0010, (&readfds)->fds_bits[0], 1);

    return;
}

/**
 * @tc.name     : fd_set_0020
 * @tc.desc     : test FD_SET with fd < 0
 * @tc.level    : Level 2
 */
static void fd_set_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int status;
    int pid = fork();
    fd_set readfds;
    int sockfd = -1;
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);
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
 * @tc.name     : fd_set_0030
 * @tc.desc     : test FD_SET with fd >= 1024
 * @tc.level    : Level 2
 */
static void fd_set_0030(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int status;
    int pid = fork();
    fd_set readfds;
    int sockfd = 1024;
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);
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
 * @tc.name     : fd_clr_0010
 * @tc.desc     : test FD_CLR with normal fd
 * @tc.level    : Level 1
 */
static void fd_clr_0010(void)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    int sockfd = 0;
    FD_SET(sockfd, &readfds);
    FD_CLR(sockfd, &readfds);
    EXPECT_EQ(fd_set_0010, (&readfds)->fds_bits[0], 0);
    
    return;
}

/**
 * @tc.name     : fd_clr_0020
 * @tc.desc     : test FD_CLR with fd < 0
 * @tc.level    : Level 2
 */
static void fd_clr_0020(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int status;
    int pid = fork();
    fd_set readfds;
    int sockfd = -1;
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            FD_ZERO(&readfds);
            FD_CLR(sockfd, &readfds);
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
 * @tc.name     : fd_clr_0030
 * @tc.desc     : test FD_CLR with fd >= 1024
 * @tc.level    : Level 2
 */
static void fd_clr_0030(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    int status;
    int pid = fork();
    fd_set readfds;
    int sockfd = 1024;
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            FD_ZERO(&readfds);
            FD_CLR(sockfd, &readfds);
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
 * @tc.name     : fd_isset_0010
 * @tc.desc     : test FD_ISSET with normal fd
 * @tc.level    : Level 1
 */
static void fd_isset_0010(void)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    int sockfd = 0;
    FD_SET(sockfd, &readfds);
    int res = FD_ISSET(sockfd, &readfds);
    EXPECT_EQ(fd_set_0010, res, 1);
    
    return;
}

/**
 * @tc.name     : fd_isset_0020
 * @tc.desc     : test FD_ISSET with fd < 0
 * @tc.level    : Level 2
 */
static void fd_isset_0020(void)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    int sockfd = -1;
    int res = FD_ISSET(sockfd, &readfds);
    EXPECT_EQ(fd_set_0010, res, 0);
    
    return;
}

/**
 * @tc.name     : fd_isset_0030
 * @tc.desc     : test FD_ISSET with fd >= 1024
 * @tc.level    : Level 2
 */
static void fd_isset_0030(void)
{
    fd_set readfds;
    FD_ZERO(&readfds);
    int sockfd = 1024;
    int res = FD_ISSET(sockfd, &readfds);
    EXPECT_EQ(fd_set_0010, res, 0);
    
    return;
}

int main(int argc, char *argv[])
{
    remove_all_special_handler(SIGABRT);
    fd_set_0010();
    fd_set_0020();
    fd_set_0030();
    fd_clr_0010();
    fd_clr_0020();
    fd_clr_0030();
    fd_isset_0010();
    fd_isset_0020();
    fd_isset_0030();
    return t_status;
}
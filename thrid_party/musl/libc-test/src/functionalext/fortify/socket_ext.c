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
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sigchain.h>
#include "fortify_test.h"
#include "test.h"

#define SCTP_IP_LOOPBACK  htonl(0x7f000001)
#define BUF_SIZE_10 (10)
#define BUF_SIZE_100 (100)

/**
 * @tc.name     : send_and_recv_0010
 * @tc.desc     : test send and recv normal condition
 * @tc.level    : Level 0
 */
static void send_and_recv_0010(void)
{
    char buf[BUF_SIZE_100];
    int sockets[2];

    TEST(socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == 0);
    TEST(send(sockets[1], "x", 1, 0) == 1);
    TEST(recv(sockets[0], buf, sizeof buf, 0) == 1);
    TEST(buf[0]=='x', "'%c'\n", buf[0]);

    return;
}

/**
 * @tc.name     : send_0010
 * @tc.desc     : test send suppress compiler optimizations
 * @tc.level    : Level 2
 */
static void send_0010(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    size_t data_len = atoi("11");
    char buf[BUF_SIZE_10];
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            send(0, buf, data_len, 0);
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
 * @tc.name     : recv_0010
 * @tc.desc     : test recv suppress compiler optimizations
 * @tc.level    : Level 2
 */
static void recv_0010(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    size_t data_len = atoi("11");
    char buf[BUF_SIZE_10];
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            recv(0, buf, data_len, 0);
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
 * @tc.name     : sendto_and_recvfrom_0010
 * @tc.desc     : test sendto and recvfrom normal condition
 * @tc.level    : Level 0
 */
static void sendto_and_recvfrom_0010(void)
{
    struct sockaddr_in sa = { .sin_family = AF_INET };
    int sendFd, recvFd, t;
    char buf[BUF_SIZE_100];

    TEST((recvFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) >= 0);
    TEST(bind(recvFd, (void *)&sa, sizeof sa)==0);
    TEST(getsockname(recvFd, (void *)&sa, (socklen_t[]){sizeof sa}) == 0);

    TEST(setsockopt(recvFd, SOL_SOCKET, SO_RCVTIMEO,
        &(struct timeval){.tv_usec=1}, sizeof(struct timeval)) == 0);

    TEST((sendFd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) >= 0);
    sa.sin_addr.s_addr = SCTP_IP_LOOPBACK;
    TEST(sendto(sendFd, "x", 1, 0, (void *)&sa, sizeof sa) == 1);
    TEST(recvfrom(recvFd, buf, sizeof buf, 0, (void *)&sa, (socklen_t[]){sizeof sa}) == 1);
    TEST(buf[0]=='x', "'%c'\n", buf[0]);

    return;
}

/**
 * @tc.name     : sendto_0010
 * @tc.desc     : test sendto suppress compiler optimizations
 * @tc.level    : Level 2
 */
static void sendto_0010(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    size_t data_len = atoi("11");
    char buf[BUF_SIZE_10];
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            sendto(0, buf, data_len, 0, NULL, 0);
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
 * @tc.name     : recvfrom_0010
 * @tc.desc     : test recv suppress compiler optimizations
 * @tc.level    : Level 2
 */
static void recvfrom_0010(void)
{
    struct sigaction sigabrt = {
        .sa_handler = SignalHandler,
    };
    sigaction(SIGABRT, &sigabrt, NULL);

    size_t data_len = atoi("11");
    char buf[BUF_SIZE_10];
    int status;
    int pid = fork();
    switch (pid) {
        case -1:
            t_error("fork failed: %s\n", strerror(errno));
            break;
        case 0:
            recvfrom(0, buf, data_len, 0, NULL, 0);
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

int main(int argc, char *argv[]) {
    remove_all_special_handler(SIGABRT);
    send_and_recv_0010();
    send_0010();
    recv_0010();
    sendto_and_recvfrom_0010();
    sendto_0010();
    recvfrom_0010();

    return t_status;
}
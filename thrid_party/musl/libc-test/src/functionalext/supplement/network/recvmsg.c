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

#include <arpa/inet.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include "functionalext.h"

#define TEST_MSG_PORT 7788
#define TEST_BUF_SIZE 256
#define TEST_TIMEOUT 5

const char *text = "This is a simple test case.";

void fill_socket_addr(struct sockaddr_in *addr)
{
    if (addr) {
        bzero(addr, sizeof(struct sockaddr_in));
        addr->sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &addr->sin_addr);
        addr->sin_port = htons(TEST_MSG_PORT);
    }
}

int check_recv_message(int sockfd)
{
    struct timeval t;
    fd_set rset;

    int timeout = TEST_TIMEOUT;
    int ret;
    while (timeout > 0) {
        FD_ZERO(&rset);
        FD_SET(sockfd, &rset);
        t.tv_sec = 1;
        t.tv_usec = 0;

        ret = select(sockfd + 1, &rset, NULL, NULL, &t);
        if (ret == 0) {
            timeout--;
        } else if (ret < 0) {
            return ret;
        } else {
            if (FD_ISSET(sockfd, &rset)) {
                return ret;
            }
        }
    }
    return -1;
}

void send_message(int flag, const char *msg)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sockfd == -1) {
        t_error("[%s] failed\n", msg);
        return;
    }

    struct sockaddr_in addr;
    struct msghdr msg_send;
    struct iovec iov_send;
    char buf[TEST_BUF_SIZE];

    fill_socket_addr(&addr);
    memset(&msg_send, 0x0, sizeof(struct msghdr));
    memset(&iov_send, 0x0, sizeof(struct iovec));

    iov_send.iov_base = buf;
    iov_send.iov_len = TEST_BUF_SIZE;

    msg_send.msg_iovlen = 1;
    msg_send.msg_iov = &iov_send;
    msg_send.msg_control = 0;
    msg_send.msg_controllen = 0;

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(sockfd);
        t_error("[%s] failed\n", msg);
        return;
    }

    int ret = sendmsg(sockfd, &msg_send, flag);
    close(sockfd);
    EXPECT_TRUE(msg, ret > 0);
}

int recv_message(int flag, const char *msg)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sockfd == -1) {
        t_error("[%s] failed\n", msg);
        exit(1);
    }

    struct sockaddr_in addr;
    struct msghdr msg_recv;
    struct iovec iov_recv;
    char buf[TEST_BUF_SIZE];
    char control[TEST_BUF_SIZE];

    fill_socket_addr(&addr);
    memset(&msg_recv, 0x0, sizeof(struct msghdr));
    memset(&iov_recv, 0x0, sizeof(struct iovec));
    memset(buf, 0x0, sizeof(buf));

    iov_recv.iov_base = (void *)buf;
    iov_recv.iov_len = TEST_BUF_SIZE;

    msg_recv.msg_name = &addr;
    msg_recv.msg_iovlen = 1;
    msg_recv.msg_iov = &iov_recv;
    msg_recv.msg_control = control;
    msg_recv.msg_controllen = TEST_BUF_SIZE;
    msg_recv.msg_flags = 0;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(sockfd);
        t_error("[%s] failed\n", msg);
        exit(1);
    }

    int ret = check_recv_message(sockfd);
    EXPECT_TRUE(msg, ret > 0);
    if (ret > 0) {
        ret = recvmsg(sockfd, &msg_recv, flag);
        EXPECT_TRUE(msg, ret > 0);
        EXPECT_STREQ(msg, iov_recv.iov_base, text);
    }
    close(sockfd);
    exit(0);
}

/**
 * @tc.name      : recvmsg_0100
 * @tc.desc      : Test recvmsg to recv messages through socket
 * @tc.level     : Level 0
 */
void recvmsg_0100(void)
{
    pid_t pid = fork();
    if (pid == 0) {
        recv_message(0, "recvmsg_0100");
    } else if (pid > 0) {
        sleep(1);
        send_message(0, "recvmsg_0100");
        wait(NULL);
    } else {
        t_error("recvmsg_0100 fork error");
    }
}

int main(void)
{
    recvmsg_0100();
    return t_status;
}
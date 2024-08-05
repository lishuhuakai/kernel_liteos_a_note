/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <bits/ioctl.h>
#include <net/if.h>
#include <resolv.h>
#include <arpa/nameser.h>
#include <poll.h>
#include <pthread.h>
#include "functionalext.h"

#define BUF_SIZE (100)
#define PORT 2288
#define WAIT() pthread_barrier_wait(&g_barrier)
static const char *g_cliMsg = "Hello, this is client";
static const char *g_localHost = "127.0.0.1";
static pthread_barrier_t g_barrier;
int bufSize = 0;

void *SampleServerTask()
{
    int *sign = (int *)malloc(sizeof(int));
    int rets = -1;
    int sListen = socket(AF_INET, SOCK_STREAM, 0);
    if (sListen == -1)
    {
        printf("[Server] socket fail!\n");
        *sign = -1;
        return sign;
    }
    int flag = 1;
    rets = setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (rets != 0)
    {
        printf("[server] setsockopt fail, rets: %d!\n", rets);
    }
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    rets = bind(sListen, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (rets != 0)
    {
        printf("[Server] bind fail!\n");
        close(sListen);
        *sign = -1;
        return sign;
    }
    rets = listen(sListen, 2);
    if (rets != 0)
    {
        printf("[Server] listen fail!\n");
        close(sListen);
        *sign = -1;
        return sign;
    }
    WAIT();
    struct sockaddr_in clnAddr = {0};
    socklen_t clnAddrLen = sizeof(clnAddr);
    int sClient = accept(sListen, (struct sockaddr *)&clnAddr, &clnAddrLen);
    printf("[server] accept <%s:%d>\n", inet_ntoa(clnAddr.sin_addr), ntohs(clnAddr.sin_port));
    static char buf[BUF_SIZE + 1] = {0};
    memset(buf, '\0', BUF_SIZE);
    rets = recv(sClient, buf, sizeof(buf), 0);
    EXPECT_EQ("recv_0100", rets, bufSize);
    close(sClient);
    close(sListen);
    return sign;
}

void *SampleClientTask()
{
    int clnFd = socket(AF_INET, SOCK_STREAM, 0);
    WAIT();
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    int sign = connect(clnFd, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    EXPECT_EQ("recv_0100", 0, sign);
    if (sign == 0)
    {
        printf("[client] connect success\n");
    }
    static char buf[BUF_SIZE + 1] = {0};
    memset(buf, '\0', BUF_SIZE);
    strcpy(buf, g_cliMsg);
    bufSize = sizeof(buf);
    int sendRet = send(clnFd, buf, sizeof(buf), 0);
    EXPECT_EQ("recv_0100", sendRet, sizeof(buf));
    close(clnFd);
    return NULL;
}

/*
 * @tc.name      : recv_0100
 * @tc.desc      : Verify that the server recv successfully
 * @tc.level     : Level 0
 */
void recv_0100()
{
    pthread_t srv;
    pthread_t cli;

    int sign = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("recv_0100", 0, sign);

    sign = pthread_create(&srv, NULL, SampleServerTask, NULL);
    EXPECT_EQ("recv_0100", 0, sign);
    sign = pthread_create(&cli, NULL, SampleClientTask, NULL);
    EXPECT_EQ("recv_0100", 0, sign);

    sign = pthread_join(cli, NULL);
    EXPECT_EQ("recv_0100", 0, sign);
    sign = pthread_join(srv, NULL);
    EXPECT_EQ("recv_0100", 0, sign);

    sign = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("recv_0100", 0, sign);
}

int main(int argc, char *argv[])
{
    recv_0100();
    return t_status;
}
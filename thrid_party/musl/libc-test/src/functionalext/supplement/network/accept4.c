/*
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

#include <arpa/inet.h>
#include <ctype.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "functionalext.h"

const int SUCCESS = 0;
#define BUF_SIZE (100)
#define PORT 2288
#define WAIT() pthread_barrier_wait(&g_barrier)
static const char *g_localHost = "127.0.0.1";
static pthread_barrier_t g_barrier;

void *ServerTaskOne()
{
    int *ret = (int *)malloc(sizeof(int));
    int rets = -1;
    int sListen = socket(AF_INET, SOCK_STREAM, 0);
    if (sListen == -1) {
        *ret = -1;
        return ret;
    }
    int flag = 1;
    rets = setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (rets != 0) {
        printf("[server] setsockopt fail, rets: %d!\n", rets);
    }
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    rets = bind(sListen, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (rets != 0) {
        close(sListen);
        *ret = -1;
        return ret;
    }
    rets = listen(sListen, 2);
    if (rets != 0) {
        close(sListen);
        *ret = -1;
        return ret;
    }
    WAIT();
    struct sockaddr_in clnAddr = {0};
    socklen_t clnAddrLen = sizeof(clnAddr);
    int sClient = -1;
    sClient = accept4(sListen, (struct sockaddr *)&clnAddr, &clnAddrLen, 0);
    EXPECT_NE("accept4_0100", sClient, -1);
    close(sClient);
    close(sListen);
    return ret;
}

void *ServerTaskTwo()
{
    int *ret = (int *)malloc(sizeof(int));
    int rets = -1;
    int sListen = socket(AF_INET, SOCK_STREAM, 0);
    if (sListen == -1) {
        *ret = -1;
        return ret;
    }
    int flag = 1;
    rets = setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (rets != 0) {
        printf("[server] setsockopt fail, rets: %d!\n", rets);
    }
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    rets = bind(sListen, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (rets != 0) {
        close(sListen);
        *ret = -1;
        return ret;
    }
    rets = listen(sListen, 2);
    if (rets != 0) {
        close(sListen);
        *ret = -1;
        return ret;
    }
    WAIT();
    struct sockaddr_in clnAddr = {0};
    socklen_t clnAddrLen = sizeof(clnAddr);
    int sClient = -1;
    sClient = accept4(sListen, (struct sockaddr *)&clnAddr, &clnAddrLen, SOCK_NONBLOCK);
    EXPECT_NE("accept4_0200", sClient, -1);
    close(sClient);
    close(sListen);
    return ret;
}

void *ServerTaskThree()
{
    int *ret = (int *)malloc(sizeof(int));
    int rets = -1;
    int sListen = socket(AF_INET, SOCK_STREAM, 0);
    if (sListen == -1) {
        *ret = -1;
        return ret;
    }
    int flag = 1;
    rets = setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (rets != 0) {
        printf("[server] setsockopt fail, rets: %d!\n", rets);
    }
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    rets = bind(sListen, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (rets != 0) {
        close(sListen);
        *ret = -1;
        return ret;
    }
    rets = listen(sListen, 2);
    if (rets != 0) {
        close(sListen);
        *ret = -1;
        return ret;
    }
    WAIT();
    struct sockaddr_in clnAddr = {0};
    socklen_t clnAddrLen = sizeof(clnAddr);
    int sClient = -1;
    sClient = accept4(sListen, (struct sockaddr *)&clnAddr, &clnAddrLen, SOCK_CLOEXEC);
    EXPECT_NE("accept4_0300", sClient, -1);
    close(sClient);
    close(sListen);
    return ret;
}

void *ServerTaskFive()
{
    int *ret = (int *)malloc(sizeof(int));
    int rets = -1;
    int sListen = socket(AF_INET, SOCK_STREAM, 0);
    if (sListen == -1) {
        *ret = -1;
        return ret;
    }
    int flag = 1;
    rets = setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (rets != 0) {
        printf("[server] setsockopt fail, rets: %d!\n", rets);
    }
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    rets = bind(sListen, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (rets != 0) {
        close(sListen);
        *ret = -1;
        return ret;
    }
    rets = listen(sListen, 2);
    if (rets != 0) {
        close(sListen);
        *ret = -1;
        return ret;
    }
    WAIT();
    struct sockaddr_in clnAddr = {0};
    socklen_t clnAddrLen = sizeof(clnAddr);
    int sClient = -1;
    sClient = accept4(sListen, 0, &clnAddrLen, SOCK_CLOEXEC);
    EXPECT_NE("accept4_0500", sClient, -1);
    close(sClient);
    close(sListen);
    return ret;
}

void *ServerTaskSix()
{
    int *ret = (int *)malloc(sizeof(int));
    int rets = -1;
    int sListen = socket(AF_INET, SOCK_STREAM, 0);
    if (sListen == -1) {
        *ret = -1;
    }
    int flag = 1;
    rets = setsockopt(sListen, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if (rets != 0) {
        printf("[server] setsockopt fail, rets: %d!\n", rets);
    }
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    rets = bind(sListen, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (rets != 0) {
        close(sListen);
        *ret = -1;
    }
    rets = listen(sListen, 2);
    if (rets != 0) {
        close(sListen);
        *ret = -1;
    }
    WAIT();
    struct sockaddr_in clnAddr = {0};
    socklen_t clnAddrLen = sizeof(clnAddr);
    int sClient = 0;
    sClient = accept4(sListen, (struct sockaddr *)&clnAddr, 0, 0);
    EXPECT_EQ("accept4_0600", sClient, -1);
    close(sClient);
    close(sListen);
    return ret;
}

void *ClientTask()
{
    int clnFd = socket(AF_INET, SOCK_STREAM, 0);
    WAIT();
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    int ret = connect(clnFd, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    if (ret != 0) {
        printf("connect failed\n");
    }
    close(clnFd);
    return NULL;
}

/**
 * @tc.name      : accept4_0100
 * @tc.desc      : Each parameter is valid, the flag parameter is 0, and the socket connection can be accepted.
 * @tc.level     : Level 0
 */
void accept4_0100(void)
{
    pthread_t srv;
    pthread_t cli;
    int ret = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("accept4_0100", SUCCESS, ret);
    ret = pthread_create(&srv, NULL, ServerTaskOne, NULL);
    EXPECT_EQ("accept4_0100", SUCCESS, ret);
    ret = pthread_create(&cli, NULL, ClientTask, NULL);
    EXPECT_EQ("accept4_0100", SUCCESS, ret);
    ret = pthread_join(cli, NULL);
    EXPECT_EQ("accept4_0100", SUCCESS, ret);
    ret = pthread_join(srv, NULL);
    EXPECT_EQ("accept4_0100", SUCCESS, ret);
    ret = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("accept4_0100", SUCCESS, ret);
}

/**
 * @tc.name      : accept4_0200
 * @tc.desc      : Each parameter is valid, the flag parameter is SOCK_NONBLOCK, which can accept socket connection.
 * @tc.level     : Level 1
 */
void accept4_0200(void)
{
    pthread_t srv;
    pthread_t cli;
    int ret = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("accept4_0200", SUCCESS, ret);
    ret = pthread_create(&srv, NULL, ServerTaskTwo, NULL);
    EXPECT_EQ("accept4_0200", SUCCESS, ret);
    ret = pthread_create(&cli, NULL, ClientTask, NULL);
    EXPECT_EQ("accept4_0200", SUCCESS, ret);
    ret = pthread_join(cli, NULL);
    EXPECT_EQ("accept4_0200", SUCCESS, ret);
    ret = pthread_join(srv, NULL);
    EXPECT_EQ("accept4_0200", SUCCESS, ret);
    ret = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("accept4_0200", SUCCESS, ret);
}

/**
 * @tc.name      : accept4_0300
 * @tc.desc      : Each parameter is valid, the flag parameter is SOCK_CLOEXEC, which can accept socket connection.
 * @tc.level     : Level 1
 */
void accept4_0300(void)
{
    pthread_t srv;
    pthread_t cli;
    int ret = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("accept4_0300", SUCCESS, ret);
    ret = pthread_create(&srv, NULL, ServerTaskThree, NULL);
    EXPECT_EQ("accept4_0300", SUCCESS, ret);
    ret = pthread_create(&cli, NULL, ClientTask, NULL);
    EXPECT_EQ("accept4_0300", SUCCESS, ret);
    ret = pthread_join(cli, NULL);
    EXPECT_EQ("accept4_0300", SUCCESS, ret);
    ret = pthread_join(srv, NULL);
    EXPECT_EQ("accept4_0300", SUCCESS, ret);
    ret = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("accept4_0300", SUCCESS, ret);
}

/**
 * @tc.name      : accept4_0400
 * @tc.desc      : The fd parameter is invalid, socket connection cannot be accepted.
 * @tc.level     : Level 2
 */
void accept4_0400(void)
{
    struct sockaddr_in clnAddr = {0};
    socklen_t clnAddrLen = sizeof(clnAddr);
    int sClient = 0;
    sClient = accept4(0, (struct sockaddr *)&clnAddr, &clnAddrLen, 0);
    EXPECT_EQ("accept4_0400", sClient, -1);
}

/**
 * @tc.name      : accept4_0500
 * @tc.desc      : The addr parameter is invalid, socket connection can be accepted.
 * @tc.level     : Level 1
 */
void accept4_0500(void)
{
    pthread_t srv;
    pthread_t cli;
    int ret = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("accept4_0500", SUCCESS, ret);
    ret = pthread_create(&srv, NULL, ServerTaskFive, NULL);
    EXPECT_EQ("accept4_0500", SUCCESS, ret);
    ret = pthread_create(&cli, NULL, ClientTask, NULL);
    EXPECT_EQ("accept4_0500", SUCCESS, ret);
    ret = pthread_join(cli, NULL);
    EXPECT_EQ("accept4_0500", SUCCESS, ret);
    ret = pthread_join(srv, NULL);
    EXPECT_EQ("accept4_0500", SUCCESS, ret);
    ret = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("accept4_0500", SUCCESS, ret);
}

/**
 * @tc.name      : accept4_0600
 * @tc.desc      : The len parameter is invalid, socket connection cannot be accepted.
 * @tc.level     : Level 2
 */
void accept4_0600(void)
{
    pthread_t srv;
    pthread_t cli;
    int ret = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("accept4_0600", SUCCESS, ret);
    ret = pthread_create(&srv, NULL, ServerTaskSix, NULL);
    EXPECT_EQ("accept4_0600", SUCCESS, ret);
    ret = pthread_create(&cli, NULL, ClientTask, NULL);
    EXPECT_EQ("accept4_0600", SUCCESS, ret);
    ret = pthread_join(cli, NULL);
    EXPECT_EQ("accept4_0600", SUCCESS, ret);
    ret = pthread_join(srv, NULL);
    EXPECT_EQ("accept4_0600", SUCCESS, ret);
    ret = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("accept4_0600", SUCCESS, ret);
}
int main(int argc, char *argv[])
{
    accept4_0100();
    accept4_0200();
    accept4_0300();
    accept4_0400();
    accept4_0500();
    accept4_0600();
    return t_status;
}
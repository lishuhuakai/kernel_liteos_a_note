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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <poll.h>
#include <pthread.h>
#include "functionalext.h"

#define PORT 2288
static const char *g_localHost = "127.0.0.1";
#define BUF_SIZE (100)
static pthread_barrier_t g_barrier;
#define WAIT() pthread_barrier_wait(&g_barrier)
typedef void (*TEST_FUN)();
void *SampleServerTask()
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
    int sClient = accept(sListen, (struct sockaddr *)&clnAddr, &clnAddrLen);
    struct sockaddr addr = {0};
    socklen_t addrLen = sizeof(addr);
    rets = getpeername(sClient, &addr, &addrLen);
    static char recordAddr[BUF_SIZE + 1] = {0};
    memset(recordAddr, '\0', BUF_SIZE);
    strcpy(recordAddr, inet_ntoa(((struct sockaddr_in *)&addr)->sin_addr));
    EXPECT_STREQ("getpeername_0100", g_localHost, recordAddr);
    EXPECT_EQ("getpeername_0100", 0, rets);
    close(sClient);
    close(sListen);
    return ret;
}

void *SampleClientTask()
{
    int clnFd = socket(AF_INET, SOCK_STREAM, 0);
    WAIT();
    struct sockaddr_in srvAddr = {0};
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_addr.s_addr = inet_addr(g_localHost);
    srvAddr.sin_port = htons(PORT);
    int ret = connect(clnFd, (struct sockaddr *)&srvAddr, sizeof(srvAddr));
    EXPECT_EQ("getpeername_0100", 0, ret);
    close(clnFd);
    return NULL;
}

/*
 * @tc.name      : getpeername_0100
 * @tc.desc      : Get the address associated with the socket
 * @tc.level     : Level 0
 */
void getpeername_0100()
{
    pthread_t srv;
    pthread_t cli;
    int ret = pthread_barrier_init(&g_barrier, 0, 2);
    EXPECT_EQ("getpeername_0100", 0, ret);
    ret = pthread_create(&srv, NULL, SampleServerTask, NULL);
    EXPECT_EQ("getpeername_0100", 0, ret);
    ret = pthread_create(&cli, NULL, SampleClientTask, NULL);
    EXPECT_EQ("getpeername_0100", 0, ret);
    ret = pthread_join(cli, NULL);
    EXPECT_EQ("getpeername_0100", 0, ret);
    ret = pthread_join(srv, NULL);
    EXPECT_EQ("getpeername_0100", 0, ret);
    ret = pthread_barrier_destroy(&g_barrier);
    EXPECT_EQ("getpeername_0100", 0, ret);
}

TEST_FUN G_Fun_Array[] = {getpeername_0100};
int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
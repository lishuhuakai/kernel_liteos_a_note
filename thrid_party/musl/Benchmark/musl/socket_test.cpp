/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
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

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "util.h"

constexpr int PORT = 1500; // port
constexpr int PIPE_FD_COUNT = 2;
constexpr int BUFFER_SIZE = 100;
constexpr int MSG_COUNT = 10;

static void Bm_function_socket_server(benchmark::State &state)
{
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any IP address of this host
    serverAddr.sin_port = htons(PORT);
    int eight = 8;
    bzero(&(serverAddr.sin_zero), eight); // Set other attributes to 0
    for (auto _ : state) {
        int serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverFd == -1) {
            printf("socket failed:%d", errno);
            break;
        }
        close(serverFd);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_socketpair_sendmsg_recvmsg(benchmark::State &state)
{
    int ret;
    int socks[2];
    struct msghdr msg;
    struct iovec iov[1];
    char sendBuf[BUFFER_SIZE] = "it is a test";
    struct msghdr msgr;
    struct iovec iovr[1];
    char recvBuf[BUFFER_SIZE];

    for (auto _ : state) {
        ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);
        if (ret == -1) {
            printf("socketpair sendmsg err\n");
            break;
        }

        bzero(&msg, sizeof(msg));
        msg.msg_name = nullptr;
        msg.msg_namelen = 0;
        iov[0].iov_base = sendBuf;
        iov[0].iov_len = sizeof(sendBuf);
        msg.msg_iov = iov;
        msg.msg_iovlen = 1;

        ret = sendmsg(socks[1], &msg, 0);
        if (ret == -1) {
            printf("sendmsg err\n");
            close(socks[0]);
            close(socks[1]);
            break;
        }

        bzero(&msgr, sizeof(msgr));
        msgr.msg_name = nullptr;
        msgr.msg_namelen = 0;
        iovr[0].iov_base = &recvBuf;
        iovr[0].iov_len = sizeof(recvBuf);
        msgr.msg_iov = iovr;
        msgr.msg_iovlen = 1;
        ret = recvmsg(socks[0], &msgr, 0);
        if (ret == -1) {
            printf("recvmsg err\n");
        }
        close(socks[0]);
        close(socks[1]);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_socketpair_sendmmsg_recvmmsg(benchmark::State &state)
{
    int ret;
    int socks[2];
    struct mmsghdr msgs[MSG_COUNT];
    struct iovec iovs[MSG_COUNT][1];
    char sendBuf[BUFFER_SIZE] = "it is a test";
    struct mmsghdr msgsr[MSG_COUNT];
    struct iovec iovsr[MSG_COUNT][1];
    char recvBuf[BUFFER_SIZE];

    for (auto _ : state) {
        ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);
        if (ret == -1) {
            printf("socketpair sendmsg err\n");
            break;
        }

        for (int i = 0; i < MSG_COUNT; i++) {
            bzero(&msgs[i], sizeof(msgs[i]));
            msgs[i].msg_hdr.msg_name = nullptr;
            msgs[i].msg_hdr.msg_namelen = 0;
            iovs[i][0].iov_base = sendBuf;
            iovs[i][0].iov_len = sizeof(sendBuf);
            msgs[i].msg_hdr.msg_iov = iovs[i];
            msgs[i].msg_hdr.msg_iovlen = 1;
        }

        ret = sendmmsg(socks[1], msgs, MSG_COUNT, 0);
        if (ret == -1) {
            printf("sendmmsg err\n");
            close(socks[0]);
            close(socks[1]);
            break;
        }

        for (int i = 0; i < MSG_COUNT; i++) {
            bzero(&msgsr[i], sizeof(msgsr[i]));
            msgsr[i].msg_hdr.msg_name = nullptr;
            msgsr[i].msg_hdr.msg_namelen = 0;
            iovsr[i][0].iov_base = &recvBuf;
            iovsr[i][0].iov_len = sizeof(recvBuf);
            msgsr[i].msg_hdr.msg_iov = iovsr[i];
            msgsr[i].msg_hdr.msg_iovlen = 1;
        }
        ret = recvmmsg(socks[0], msgsr, MSG_COUNT, 0, nullptr);
        if (ret == -1) {
            printf("recvmmsg err\n");
        }
        close(socks[0]);
        close(socks[1]);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_socketpair_sendto_recvfrom(benchmark::State &state)
{
    int ret;
    int socks[2];
    char sendBuf[BUFFER_SIZE] = "it is a test";
    char recvBuf[BUFFER_SIZE];

    for (auto _ : state) {
        ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);
        if (ret == -1) {
            printf("socketpair sendto err\n");
            break;
        }

        ret = sendto(socks[1], sendBuf, sizeof(sendBuf), 0, nullptr, 0);
        if (ret == -1) {
            printf("sendto err\n");
            close(socks[0]);
            close(socks[1]);
            break;
        }

        ret = recvfrom(socks[0], recvBuf, sizeof(recvBuf), 0, nullptr, 0);
        if (ret == -1) {
            printf("recvfrom err\n");
        }
        close(socks[0]);
        close(socks[1]);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_getsockopt(benchmark::State &state)
{
    int optVal;
    socklen_t optLen = sizeof(optVal);

    for (auto _ : state) {
        int sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (getsockopt(sockFd, SOL_SOCKET, SO_SNDBUF, &optVal, &optLen) < 0) {
            printf("fail to getsockopt");
        }
        close(sockFd);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_setsockopt(benchmark::State &state)
{
    for (auto _ : state) {
        int nRecvBuf = 32*1024; // Set to 32K
        int sockFd = socket(AF_INET, SOCK_STREAM, 0);
        if (setsockopt(sockFd, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int)) < 0) {
            printf("fail to setsockopt");
        }
        close(sockFd);
    }
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_sockpair(benchmark::State &state)
{
    for (auto _ : state) {
        int fd[PIPE_FD_COUNT] = {-1, -1};
        benchmark::DoNotOptimize(socketpair(0, SOCK_STREAM, 0, fd));
        state.PauseTiming();
        for (size_t i = 0; i < PIPE_FD_COUNT; i++) {
            if (fd[i] >= 0) {
                close(fd[i]);
            }
        }
        state.ResumeTiming();
    }
}

static void Bm_function_pipe(benchmark::State &state)
{
    for (auto _ : state) {
        int fd[PIPE_FD_COUNT] = {-1, -1};
        benchmark::DoNotOptimize(pipe(fd));
        state.PauseTiming();
        for (size_t i = 0; i < PIPE_FD_COUNT; i++) {
            if (fd[i] >= 0) {
                close(fd[i]);
            }
        }
        state.ResumeTiming();
    }
}

static int ConnectTo(const char *host, const char *port, int socktype, struct addrinfo *addr)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = 0;
    hint.ai_family = AF_UNSPEC;
    hint.ai_protocol = 0;
    hint.ai_socktype = socktype;

    int n = getaddrinfo(host, port, &hint, &res);
    if (n != 0) {
        perror("getaddrinfo");
        return -1;
    }

    int sfd = -1;
    for (struct addrinfo *rp = res; rp != nullptr; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            if (addr != nullptr) {
                struct sockaddr *aiAddr = addr->ai_addr;
                *addr = *rp;
                *aiAddr = *rp->ai_addr;
                addr->ai_addr = aiAddr;
            }
            break;
        }

        close(sfd);
        sfd = -1;
    }

    freeaddrinfo(res);

    if (sfd == -1) {
        perror("connect");
    }

    return sfd;
}

static int BindAt(const char *port)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_PASSIVE;
    hint.ai_family = AF_UNSPEC;
    hint.ai_protocol = 0;
    hint.ai_socktype = SOCK_STREAM;

    int n = getaddrinfo("127.0.0.1", port, &hint, &res);
    if (n != 0) {
        perror("getaddrinfo");
        return -1;
    }

    int sfd = -1;
    for (struct addrinfo *rp = res; rp != nullptr; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }

        int reuse = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
            close(sfd);
            continue;
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;
        }

        close(sfd);
        sfd = -1;
    }

    freeaddrinfo(res);

    if (sfd == -1) {
        perror("bind");
    }

    return sfd;
}

static void Bm_function_getpeername_client_local(benchmark::State &state)
{
    int sfd = ConnectTo("127.0.0.1", "80", SOCK_DGRAM, nullptr);
    if (sfd == -1) {
    }

    struct sockaddr_in peerAddr;
    socklen_t peerLen = sizeof(peerAddr);
    for (auto _ : state) {
        if (getpeername(sfd, (struct sockaddr *)&peerAddr, &peerLen) == -1) {
            perror("getpeername");
            printf("peer addr: %s:%d\n", inet_ntoa(peerAddr.sin_addr), ntohs(peerAddr.sin_port));
        }
    }

    close(sfd);
}

static void Bm_function_getpeername_client_external(benchmark::State &state)
{
    int sfd = ConnectTo("www.baidu.com", "80", SOCK_DGRAM, nullptr);
    if (sfd == -1) {
    }

    struct sockaddr_in peerAddr;
    socklen_t peerLen = sizeof(peerAddr);
    for (auto _ : state) {
        if (getpeername(sfd, (struct sockaddr *)&peerAddr, &peerLen) == -1) {
            perror("getpeername");
            printf("peer addr: %s:%d\n", inet_ntoa(peerAddr.sin_addr), ntohs(peerAddr.sin_port));
        }
    }

    close(sfd);
}

void* ThreadTaskClient(void* arg)
{
    int sfd = ConnectTo("127.0.0.1", "1500", SOCK_STREAM, nullptr);
    if (sfd == -1) {
        return (void*)-1;
    }

    close(sfd);
    return nullptr;
}

static int StartServer(int *connfd)
{
    int sfd = BindAt("1500");
    if (sfd == -1) {
        return -1;
    }

    if (listen(sfd, 1) < 0) {
        perror("listen");
        return -1;
    }

    pthread_t thread;
    void *res;
    pthread_create(&thread, nullptr, ThreadTaskClient, nullptr);
    pthread_join(thread, &res);
    if (res == (void*)-1) {
        return -1;
    }

    *connfd = accept(sfd, nullptr, nullptr);
    if (*connfd < 0) {
        perror("accept");
        return -1;
    }

    return sfd;
}

static void Bm_function_getpeername_server(benchmark::State &state)
{
    int connfd;
    int sfd = StartServer(&connfd);
    if (sfd < 0) {
    }

    struct sockaddr_in peerAddr;
    socklen_t peerLen = sizeof(peerAddr);
    for (auto _ : state) {
        if (getpeername(connfd, (struct sockaddr *)&peerAddr, &peerLen) == -1) {
            perror("getpeername");
            printf("peer addr: %s:%d\n", inet_ntoa(peerAddr.sin_addr), ntohs(peerAddr.sin_port));
        }
    }

    close(sfd);
}

static void Bm_function_getsockname_client_local(benchmark::State &state)
{
    int sfd = ConnectTo("127.0.0.1", "80", SOCK_DGRAM, nullptr);
    if (sfd == -1) {
    }

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    for (auto _ : state) {
        if (getsockname(sfd, (struct sockaddr *)&addr, &len) == -1) {
            perror("getsockname");
            printf("sock addr: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        }
    }

    close(sfd);
}

static void Bm_function_getsockname_client_external(benchmark::State &state)
{
    int sfd = ConnectTo("www.baidu.com", "80", SOCK_DGRAM, nullptr);
    if (sfd == -1) {
    }

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    for (auto _ : state) {
        if (getsockname(sfd, (struct sockaddr *)&addr, &len) == -1) {
            perror("getsockname");
            printf("sock addr: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        }
    }

    close(sfd);
}

static void Bm_function_getsockname_server(benchmark::State &state)
{
    int connfd;
    int sfd = StartServer(&connfd);
    if (sfd < 0) {
    }

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    for (auto _ : state) {
        if (getsockname(connfd, (struct sockaddr *)&addr, &len) == -1) {
            perror("getsockname");
            printf("sock addr: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        }
    }

    close(sfd);
}

static void Bm_function_connect_local_dgram(benchmark::State &state)
{
    struct addrinfo addr;
    struct sockaddr_in aiAddr;
    addr.ai_addr = (struct sockaddr*)&aiAddr;
    int sfd = ConnectTo("127.0.0.1", "80", SOCK_DGRAM, &addr);
    if (sfd < 0) {
    }

    for (auto _ : state) {
        if (connect(sfd, addr.ai_addr, addr.ai_addrlen) == -1) {
            perror("connect");
        }
    }

    close(sfd);
}

static void Bm_function_connect_accept(benchmark::State &state)
{
    int sfd = BindAt("1500");
    if (sfd == -1) {
    }

    if (listen(sfd, 1) < 0) {
        perror("listen");
    }

    struct addrinfo addr;
    struct sockaddr_in aiAddr;
    addr.ai_addr = (struct sockaddr*)&aiAddr;
    int sfd2 = ConnectTo("127.0.0.1", "1500", SOCK_STREAM, &addr);
    if (sfd2 == -1) {
    }

    for (auto _ : state) {
        int connfd = accept(sfd, nullptr, nullptr);
        if (connfd < 0) {
            perror("accept");
        }

        state.PauseTiming();
        close(connfd);
        close(sfd2);
        sfd2 = socket(addr.ai_family, addr.ai_socktype, addr.ai_protocol);
        if (sfd == -1) {
            perror("socket");
        }
        if (connect(sfd2, addr.ai_addr, addr.ai_addrlen) == -1) {
            perror("connect2");
        }
        state.ResumeTiming();
    }

    close(sfd);
}

static void Bm_function_bind(benchmark::State &state)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    for (auto _ : state) {
        state.PauseTiming();
        int sfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sfd == -1) {
            perror("socket");
        }

        int reuse = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
            perror("setsockopt");
            close(sfd);
        }

        state.ResumeTiming();

        if (bind(sfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            perror("bind");
        }

        state.PauseTiming();
        close(sfd);
        state.ResumeTiming();
    }
}

MUSL_BENCHMARK(Bm_function_socket_server);
MUSL_BENCHMARK(Bm_function_socketpair_sendmsg_recvmsg);
MUSL_BENCHMARK(Bm_function_socketpair_sendmmsg_recvmmsg);
MUSL_BENCHMARK(Bm_function_socketpair_sendto_recvfrom);
MUSL_BENCHMARK(Bm_function_getsockopt);
MUSL_BENCHMARK(Bm_function_setsockopt);
MUSL_BENCHMARK(Bm_function_sockpair);
MUSL_BENCHMARK(Bm_function_pipe);
MUSL_BENCHMARK(Bm_function_getpeername_client_local);
MUSL_BENCHMARK(Bm_function_getpeername_client_external);
MUSL_BENCHMARK(Bm_function_getpeername_server);
MUSL_BENCHMARK(Bm_function_getsockname_client_local);
MUSL_BENCHMARK(Bm_function_getsockname_client_external);
MUSL_BENCHMARK(Bm_function_getsockname_server);
MUSL_BENCHMARK(Bm_function_connect_local_dgram);
MUSL_BENCHMARK(Bm_function_connect_accept);
MUSL_BENCHMARK(Bm_function_bind);

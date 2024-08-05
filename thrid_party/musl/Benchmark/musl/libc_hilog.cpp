/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
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
#include <stdarg.h>
#include <hilog_adapter.h>
#include <string.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/uio.h>
#include "util.h"
#define SOCKET_FILE_DIR "/dev/unix/socket/"
#define INPUT_SOCKET_NAME "hilogInput"

#define MUSL_LOG_TYPE LOG_CORE
#define MUSL_LOG_DOMAIN 0xD003F00
#define MUSL_LOG_TAG "MUSL"
#define CLOSE_FD_COUNT 1024
#define LOG_LEN 3
#define ERROR_FD 2
#define MAX_LOG_LEN 1024
#define MAX_TAG_LEN 32

const int SOCKET_TYPE = SOCK_DGRAM | SOCK_NONBLOCK | SOCK_CLOEXEC;
const int INVALID_SOCKET = -1;
const struct sockaddr_un SOCKET_ADDR = {AF_UNIX, SOCKET_FILE_DIR INPUT_SOCKET_NAME};


static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static volatile int g_socketFd = INVALID_SOCKET;

/*
 * header of log message from libhilog to hilogd
 */
typedef struct __attribute__((__packed__)) {
    uint16_t len;
    uint16_t version : 3;
    uint16_t type : 4; /* APP,CORE,INIT,SEC etc */
    uint16_t level : 3;
    uint16_t tag_len : 6; /* include '\0' */
    uint32_t tv_sec;
    uint32_t tv_nsec;
    uint32_t mono_sec;
    uint32_t pid;
    uint32_t tid;
    uint32_t domain;
    char tag[]; /* shall be end with '\0' */
} HilogMsg;

static void Cleanup()
{
    if (g_socketFd >= 0) {
        close(g_socketFd);
        g_socketFd = INVALID_SOCKET;
    }
}

static int GetSocketFdInstance()
{
    if (g_socketFd == INVALID_SOCKET || fcntl(g_socketFd, F_GETFL) == -1) {
        errno = 0;
        pthread_mutex_lock(&g_lock);
        if (g_socketFd == INVALID_SOCKET || fcntl(g_socketFd, F_GETFL) == -1) {
            int tempSocketFd = TEMP_FAILURE_RETRY(socket(AF_UNIX, SOCKET_TYPE, 0));
            if (tempSocketFd < 0) {
                dprintf(ERROR_FD, "HiLogAdapter: Can't create socket! Errno: %d\n", errno);
                pthread_mutex_unlock(&g_lock);
                return tempSocketFd;
            }

            long int result =
                TEMP_FAILURE_RETRY(connect(tempSocketFd, (const struct sockaddr *)(&SOCKET_ADDR), sizeof(SOCKET_ADDR)));
            if (result < 0) {
                dprintf(ERROR_FD, "HiLogAdapter: Can't connect to server. Errno: %d\n", errno);
                if (tempSocketFd >= 0) {
                    close(tempSocketFd);
                }
                pthread_mutex_unlock(&g_lock);
                return result;
            }
            g_socketFd = tempSocketFd;
            atexit(Cleanup);
        }
        pthread_mutex_unlock(&g_lock);
    }
    return g_socketFd;
}

static int SendMessage(HilogMsg *header, const char *tag, uint16_t tagLen, const char *fmt, uint16_t fmtLen)
{
    int socketFd = GetSocketFdInstance();
    if (socketFd < 0) {
        return socketFd;
    }
    struct timespec ts = {0};
    (void)clock_gettime(CLOCK_REALTIME, &ts);
    struct timespec tsMono = {0};
    (void)clock_gettime(CLOCK_MONOTONIC, &tsMono);
    header->tv_sec = (uint32_t)(ts.tv_sec);
    header->tv_nsec = (uint32_t)(ts.tv_nsec);
    header->mono_sec = (uint32_t)(tsMono.tv_sec);
    header->len = sizeof(HilogMsg) + tagLen + fmtLen;
    header->tag_len = tagLen;

    struct iovec vec[LOG_LEN] = {{0}};
    vec[0].iov_base = header;                   // 0 : index of hos log header
    vec[0].iov_len = sizeof(HilogMsg);          // 0 : index of hos log header
    vec[1].iov_base = (void *)((char *)(tag));  // 1 : index of log tag
    vec[1].iov_len = tagLen;                    // 1 : index of log tag
    vec[2].iov_base = (void *)((char *)(fmt));  // 2 : index of log content
    vec[2].iov_len = fmtLen;                    // 2 : index of log content
    int ret = TEMP_FAILURE_RETRY(writev(socketFd, vec, LOG_LEN));
    return ret;
}

int HiLogAdapterPrintArgs1(
    const LogType type, const LogLevel level, const unsigned int domain, const char *tag, const char *fmt, va_list ap)
{
    char buf[MAX_LOG_LEN] = {0};

    vsnprintf(buf, MAX_LOG_LEN, fmt, ap);

    size_t tagLen = strnlen(tag, MAX_TAG_LEN - 1);
    size_t logLen = strnlen(buf, MAX_LOG_LEN - 1);
    HilogMsg header = {0};
    header.type = type;
    header.level = level;
#ifndef __RECV_MSG_WITH_UCRED_
    header.pid = getpid();
#endif
    header.tid = (uint32_t)(gettid());
    header.domain = domain;

    return SendMessage(&header, tag, tagLen + 1, buf, logLen + 1);
}

int HiLogAdapterPrint1(LogType type, LogLevel level, unsigned int domain, const char *tag, const char *fmt, ...)
{
    int ret;
    va_list ap;
    va_start(ap, fmt);
    ret = HiLogAdapterPrintArgs1(type, level, domain, tag, fmt, ap);
    va_end(ap);
    return ret;
}

// Used to perform control operations on device files
// set baud rate
static void Bm_function_hilog(benchmark::State &state)
{
    for (auto _ : state) {
        int ret = HiLogAdapterPrint1(MUSL_LOG_TYPE, LOG_ERROR, MUSL_LOG_DOMAIN, MUSL_LOG_TAG, "musl-hilog-benchmark");
        benchmark::DoNotOptimize(ret);
    }
}

MUSL_BENCHMARK(Bm_function_hilog);

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

#include <sys/epoll.h>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "util.h"

using namespace std;
constexpr int EVENTSIZE = 10;
struct epoll_event g_events[EVENTSIZE], g_event;

// Used to create an epoll object to record events for files to be listened to
static void Bm_function_Epoll_createl(benchmark::State &state)
{
    for (auto _ : state) {
        int epollFd = epoll_create1(0);
        if (epollFd == -1) {
            perror("epoll_createl");
        }
        benchmark::DoNotOptimize(epollFd);
        close(epollFd);
    }

    state.SetBytesProcessed(state.iterations());
}

// Used to add, modify, or delete events to an epoll object
static void Bm_function_Epoll_ctl(benchmark::State &state)
{
    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
        perror("epoll_createl");
    }
    int fd = open("/dev/zero", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open epoll_ctl");
    }
    g_event.events = EPOLLIN | EPOLLET;
    g_event.data.fd = fd;
    for (auto _ : state) {
        benchmark::DoNotOptimize(epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &g_event));
    }
    close(epollFd);
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

// Used to wait for the file descriptor in the epoll object to appear for the event
static void Bm_function_Epoll_wait(benchmark::State &state)
{
    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
        perror("epoll_createl");
    }

    int fd = open("/dev/zero", O_RDONLY, OPEN_MODE);
    if (fd == -1) {
        perror("open epoll_wait");
    }

    g_event.events = EPOLLIN | EPOLLET;
    g_event.data.fd = fd;
    for (auto _ : state) {
        epoll_wait(epollFd, g_events, EVENTSIZE, 0);
    }
    close(epollFd);
    close(fd);
    state.SetBytesProcessed(state.iterations());
}

MUSL_BENCHMARK(Bm_function_Epoll_createl);
MUSL_BENCHMARK(Bm_function_Epoll_ctl);
MUSL_BENCHMARK(Bm_function_Epoll_wait);
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

#include "sys/types.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "netdb.h"
#include "util.h"

using namespace std;

// Free up memory
static void Bm_function_Getaddrinfo_Freeaddrinfo_external_network(benchmark::State &state)
{
    struct addrinfo *res;
    for (auto _ : state) {
        int n = getaddrinfo("www.baidu.com", nullptr, nullptr, &res);
        if (n != 0) {
            perror("getaddrinfo external_network");
        }
        benchmark::DoNotOptimize(n);
        freeaddrinfo(res);
    }
    res = nullptr;
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_Freeaddrinfo_intranet1(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_PASSIVE;
    hint.ai_family = AF_UNSPEC;
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet1");
        }
        benchmark::DoNotOptimize(n);
        freeaddrinfo(res);
    }
    res = nullptr;
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_Freeaddrinfo_intranet2(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_ALL;  // Find IPv4 and IPV6
    hint.ai_family = AF_UNSPEC; // not specified
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet2");
        }
        benchmark::DoNotOptimize(n);
        freeaddrinfo(res);
    }
    res = nullptr;
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_Freeaddrinfo_intranet3(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_V4MAPPED; // If IPV6 is not found, return IPv4 mapped in ipv6 format
    hint.ai_family = AF_INET6;   // IPv6 Internet Domain
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet3");
        }
        benchmark::DoNotOptimize(n);
        freeaddrinfo(res);
    }
    res = nullptr;
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_Freeaddrinfo_intranet4(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_V4MAPPED;  // If IPV6 is not found, return IPv4 mapped in ipv6 format
    hint.ai_family = AF_UNSPEC;   // not specified
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet4");
        }
        benchmark::DoNotOptimize(n);
        freeaddrinfo(res);
    }
    res = nullptr;
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_Freeaddrinfo_intranet5(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_ADDRCONFIG; // Query the configured address type IPV4 or IPV6
    hint.ai_family = AF_UNSPEC;    // not specified
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet5");
        }
        benchmark::DoNotOptimize(n);
        freeaddrinfo(res);
    }
    res = nullptr;
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_Freeaddrinfo_intranet6(benchmark::State &state)
{
    struct addrinfo hint;
    struct addrinfo *res;
    bzero(&hint, sizeof(struct addrinfo));
    hint.ai_flags = AI_NUMERICSERV;
    hint.ai_family = AF_UNSPEC;
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, &hint, &res);
        if (n != 0) {
            perror("getaddrinfo intranet6");
        }
        benchmark::DoNotOptimize(n);
        freeaddrinfo(res);
    }
    res = nullptr;
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Getaddrinfo_Freeaddrinfo_intranet7(benchmark::State &state)
{
    struct addrinfo *res;
    for (auto _ : state) {
        int n = getaddrinfo("127.0.0.1", nullptr, nullptr, &res);
        if (n != 0) {
            perror("getaddrinfo intranet7");
        }
        benchmark::DoNotOptimize(n);
        freeaddrinfo(res);
    }
    res = nullptr;
    state.SetBytesProcessed(state.iterations());
}

static void Bm_function_Network_ntohs(benchmark::State &state)
{
    uint16_t srcPort = 5060;
    for (auto _ : state) {
        benchmark::DoNotOptimize(ntohs(srcPort));
    }
}

static void Bm_function_Inet_pton(benchmark::State &state)
{
    unsigned char buf[sizeof(struct in_addr)];
    for (auto _ : state) {
        benchmark::DoNotOptimize(inet_pton(AF_INET, "127.0.0.1", buf));
    }
}

static void Bm_function_Inet_ntop(benchmark::State &state)
{
    struct in_addr addr;
    if (inet_pton(AF_INET, "127.0.0.1", &addr.s_addr) <= 0) {
        perror("inet_pton failed");
    }
    for (auto _ : state) {
        char str[INET_ADDRSTRLEN] = {0};
        benchmark::DoNotOptimize(inet_ntop(AF_INET, &addr.s_addr, str, INET_ADDRSTRLEN));
    }
}
static void Bm_function_Inet_ntop_ipv6(benchmark::State &state)
{
    struct in6_addr addr;
    if (inet_pton(AF_INET6, "fe80::cd01:7cd8:bd13:98d5", &addr.s6_addr) <= 0) {
        perror("inet_pton failed");
    }
    for (auto _ : state) {
        char str[INET6_ADDRSTRLEN] = {0};
        benchmark::DoNotOptimize(inet_ntop(AF_INET6, &addr.s6_addr, str, INET6_ADDRSTRLEN));
    }
}
MUSL_BENCHMARK(Bm_function_Getaddrinfo_Freeaddrinfo_external_network);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_Freeaddrinfo_intranet1);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_Freeaddrinfo_intranet2);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_Freeaddrinfo_intranet3);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_Freeaddrinfo_intranet4);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_Freeaddrinfo_intranet5);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_Freeaddrinfo_intranet6);
MUSL_BENCHMARK(Bm_function_Getaddrinfo_Freeaddrinfo_intranet7);
MUSL_BENCHMARK(Bm_function_Network_ntohs);
MUSL_BENCHMARK(Bm_function_Inet_pton);
MUSL_BENCHMARK(Bm_function_Inet_ntop);
MUSL_BENCHMARK(Bm_function_Inet_ntop_ipv6);

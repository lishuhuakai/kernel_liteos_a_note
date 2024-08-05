/**
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#include "functionalext.h"

const int GETADDRINFO_RESULT = 0;
const int FLAGS_FIELD = -1;
const int SERVICE_UNKNOEN = -2;
const int FAMILY_NOTSUPPORTED = -6;
const int SOCKTYPE_NOTSUPPORTED = -8;

/**
 * @tc.name      : getaddrinfo_0100
 * @tc.desc      : Each parameter is valid, the ai_flags of hint is AI_PASSIVE, which can resolve the IP
 *                 address of the host name.
 * @tc.level     : Level 0
 */
void getaddrinfo_0100(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_PASSIVE;
    hint.ai_family = AF_UNSPEC;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_0100", ret, GETADDRINFO_RESULT);
    EXPECT_TRUE("getaddrinfo_0100", result->ai_addr != NULL);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getaddrinfo_0200
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name.
 * @tc.level     : Level 0
 */
void getaddrinfo_0200(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_ALL;
    hint.ai_family = AF_UNSPEC;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_0200", ret, GETADDRINFO_RESULT);
    EXPECT_TRUE("getaddrinfo_0200", result->ai_addr != NULL);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getaddrinfo_0300
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name
 *                 (hint.ai_flags =AI_NUMERICHOST).
 * @tc.level     : Level 0
 */
void getaddrinfo_0300(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_NUMERICHOST;
    hint.ai_family = AF_INET6;
    char buf[] = "fe80::bed5:4695:6cac:bef8";
    int ret = getaddrinfo(buf, NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_0300", ret, GETADDRINFO_RESULT);
    EXPECT_TRUE("getaddrinfo_0300", result->ai_addr != NULL);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getaddrinfo_0400
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint =AI_V4MAPPED).
 * @tc.level     : Level 0
 */
void getaddrinfo_0400(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_V4MAPPED;
    hint.ai_family = AF_INET6;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_0400", ret, GETADDRINFO_RESULT);
    EXPECT_TRUE("getaddrinfo_0400", result->ai_addr != NULL);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getaddrinfo_0500
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint =AI_V4MAPPED).
 * @tc.level     : Level 0
 */
void getaddrinfo_0500(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_V4MAPPED;
    hint.ai_family = AF_UNSPEC;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_0500", ret, GETADDRINFO_RESULT);
    EXPECT_TRUE("getaddrinfo_0500", result->ai_addr != NULL);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getaddrinfo_0600
 * @tc.desc      : Each parameter is valid and can resolve the IP address of
 *                 the host name(hint.ai_flags =AI_ADDRCONFIG).
 * @tc.level     : Level 0
 */
void getaddrinfo_0600(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_ADDRCONFIG;
    hint.ai_family = AF_UNSPEC;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_0600", ret, GETADDRINFO_RESULT);
    EXPECT_TRUE("getaddrinfo_0600", result->ai_addr != NULL);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getaddrinfo_0700
 * @tc.desc      : Each parameter is valid and can resolve the IP address
 *                 of the host name(hint.ai_flags =AI_NUMERICSERV).
 * @tc.level     : Level 1
 */
void getaddrinfo_0700(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_NUMERICSERV;
    hint.ai_family = AF_UNSPEC;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_0700", ret, GETADDRINFO_RESULT);
    EXPECT_TRUE("getaddrinfo_0700", result->ai_addr != NULL);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getaddrinfo_0800
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint =NULL).
 * @tc.level     : Level 1
 */
void getaddrinfo_0800(void)
{
    struct addrinfo *result;
    int ret = getaddrinfo("127.0.0.1", NULL, NULL, &result);
    EXPECT_EQ("getaddrinfo_0800", ret, GETADDRINFO_RESULT);
    EXPECT_TRUE("getaddrinfo_0800", result->ai_addr != NULL);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getaddrinfo_0900
 * @tc.desc      : Invalid parameter, Hint’s ai_flags is invalid, can not resolve the IP address of the host name.
 * @tc.level     : Level 2
 */
void getaddrinfo_0900(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = 0x4000;
    hint.ai_family = AF_UNSPEC;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_0900", ret, FLAGS_FIELD);
}

/**
 * @tc.name      : getaddrinfo_1000
 * @tc.desc      : Invalid parameter, Hint’s ai_family is invalid, can not resolve the IP address of the host name.
 * @tc.level     : Level 2
 */
void getaddrinfo_1000(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_ALL;
    hint.ai_family = PF_AX25;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_1000", ret, FAMILY_NOTSUPPORTED);
}

/**
 * @tc.name      : getaddrinfo_1100
 * @tc.desc      : The parameters are invalid,hint ai_flags is AI_NUMERICHOST,
 *                 host format is incorrect, can not resolve the host name Ip address.
 * @tc.level     : Level 2
 */
void getaddrinfo_1100(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_NUMERICHOST;
    hint.ai_family = AF_INET6;
    char one[300] = "fe80::bed5:4695:6cac:bef8:4695:6cac:bef8:4695:bef8:4695:6cac:bef8:4695";
    char two[] = ":fe80::bed5:4695:6cac:bef8:4695:6cac:bef8:4695:bef8:4695:6cac:bef8:4695";
    char three[] = ":fe80::bed5:4695:6cac:bef8:4695:6cac:bef8:4695:bef8:4695:6cac:bef8:4695";
    char four[] = ":fe80::bed5:4695:6cac:bef8:4695:6cac:bef8:4695:bef8:4695:6cac:bef8:4695";
    strcat(one, two);
    strcat(one, three);
    strcat(one, four);
    int ret = getaddrinfo(one, NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_1100", ret, SERVICE_UNKNOEN);
}

/**
 * @tc.name      : getaddrinfo_1200
 * @tc.desc      : The parameter is invalid, host is NULL, SERV is NULL, and the IP address of the
 *                 host name can not be resolved.
 * @tc.level     : Level 2
 */
void getaddrinfo_1200(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_NUMERICHOST;
    hint.ai_family = AF_INET6;
    int ret = getaddrinfo(NULL, NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_1200", ret, SERVICE_UNKNOEN);
}

/**
 * @tc.name      : getaddrinfo_1300
 * @tc.desc      : The parameter is invalid and the IP address of the host name can not be resolved.
 * @tc.level     : Level 2
 */
void getaddrinfo_1300(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = AF_INET;
    hint.ai_protocol = IPPROTO_UDP;
    hint.ai_socktype = SOCK_STREAM;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_1300", ret, SOCKTYPE_NOTSUPPORTED);
}

/**
 * @tc.name      : getaddrinfo_1400
 * @tc.desc      : The parameter is invalid and the IP address of the host name can not be resolved.
 * @tc.level     : Level 2
 */
void getaddrinfo_1400(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = AF_INET;
    hint.ai_protocol = IPPROTO_TCP;
    hint.ai_socktype = SOCK_DGRAM;
    int ret = getaddrinfo("127.0.0.1", NULL, &hint, &result);
    EXPECT_EQ("getaddrinfo_1400", ret, SOCKTYPE_NOTSUPPORTED);
}

/**
 * @tc.name      : getaddrinfo_1500
 * @tc.desc      : The parameter is invalid and the IP address of the host name can not be resolved.
 * @tc.level     : Level 2
 */
void getaddrinfo_1500(void)
{
    struct addrinfo *result, hint;
    hint.ai_flags = AI_CANONNAME;
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_RAW;
    int ret = getaddrinfo("127.0.0.1", "2000", &hint, &result);
    EXPECT_EQ("getaddrinfo_1500", ret, SOCKTYPE_NOTSUPPORTED);
}

static void *test_all_cases(void *arg)
{
    getaddrinfo_0100();
    getaddrinfo_0200();
    getaddrinfo_0300();
    getaddrinfo_0400();
    getaddrinfo_0500();
    getaddrinfo_0600();
    getaddrinfo_0700();
    getaddrinfo_0800();
    getaddrinfo_0900();
    getaddrinfo_1000();
    getaddrinfo_1100();
    getaddrinfo_1200();
    getaddrinfo_1300();
    getaddrinfo_1400();
    getaddrinfo_1500();
    return arg;
}

static void do_test_concurrently(void *(*test) (void *arg), size_t num_threads)
{
    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * num_threads);
    if (threads == NULL) {
        t_error("Failed to allocate memory: %s\n", strerror(errno));
        return;
    }

    size_t last = 0;
    while (last < num_threads) {
        if (pthread_create(&(threads[last]), NULL, test, NULL)) {
            t_error("Failed to create thread: %s\n", strerror(errno));
            break;
        }
        last++;
    }

    for (size_t i = 0; i < last; i++) {
        if (pthread_join(threads[i], NULL)) {
            t_error("Failed to join thread: %s\n", strerror(errno));
        }
    }

    free(threads);
    return;
}

int main(int argc, char *argv[])
{
    size_t num_threads = 16;
    do_test_concurrently(test_all_cases, num_threads);

    return t_status;
}

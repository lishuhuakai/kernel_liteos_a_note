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
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "functionalext.h"
#define HTTP_PORT 80
#define TCPMUX_PORT 1
#define FIDO_PORT 60179
#define NOT_EXIST_PORT 62000
#define NOT_EXIST_PORT_STR "62000"

const int INFO_RESULT = 0;
const int FAMILY_NOTSUPPORTED = -6;

/**
 * @tc.name      : getnameinfo_0100
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint.ai_flags =AI_ALL).
 * @tc.level     : Level 0
 */
void getnameinfo_0100()
{
    int ret;
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr.s_addr);
    ret = getnameinfo((const struct sockaddr *)(&sa), sizeof(sa), host, sizeof(host), server, sizeof(server), 0);
    EXPECT_EQ("getnameinfo_0100", ret, INFO_RESULT);
    EXPECT_NE("getnameinfo_0100", strlen(host), 0);
    EXPECT_NE("getnameinfo_0100", strlen(server), 0);
}

/**
 * @tc.name      : getnameinfo_0200
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint.ai_flags =AI_ALL).
 * @tc.level     : Level 0
 */
void getnameinfo_0200()
{
    int ret = -1;
    int num = -1;
    int res = -1;
    struct addrinfo *result, hint;
    hint.ai_flags = AI_NUMERICHOST;
    hint.ai_family = AF_INET6;
    char buf[] = "fe80::bed5:4695:6cac:bef8";
    ret = getaddrinfo(buf, NULL, &hint, &result);
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];
    if (ret == 0) {
        res = getnameinfo(result->ai_addr, result->ai_addrlen, host, sizeof(host), server, sizeof(server), 0);
    }
    EXPECT_EQ("getnameinfo_0200", res, INFO_RESULT);
    EXPECT_NE("getnameinfo_0200", strlen(host), 0);
    EXPECT_NE("getnameinfo_0200", strlen(server), 0);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getnameinfo_0300
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint.ai_flags =AI_ALL).
 * @tc.level     : Level 1
 */
void getnameinfo_0300()
{
    int ret;
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr.s_addr);
    ret = getnameinfo((const struct sockaddr *)(&sa), sizeof(sa), 0, 0, server, sizeof(server), 0);
    EXPECT_EQ("getnameinfo_0300", ret, INFO_RESULT);
    EXPECT_NE("getnameinfo_0300", strlen(server), 0);
}

/**
 * @tc.name      : getnameinfo_0400
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint.ai_flags =AI_ALL).
 * @tc.level     : Level 1
 */
void getnameinfo_0400()
{
    int ret;
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr.s_addr);
    ret = getnameinfo((const struct sockaddr *)(&sa), sizeof(sa), host, sizeof(host), 0, 0, 0);
    EXPECT_EQ("getnameinfo_0400", ret, INFO_RESULT);
    EXPECT_NE("getnameinfo_0400", strlen(host), 0);
}
/**
 * @tc.name      : getnameinfo_0500
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint.ai_flags =AI_ALL).
 * @tc.level     : Level 2
 */
void getnameinfo_0500()
{
    int ret;
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr.s_addr);
    ret = getnameinfo((const struct sockaddr *)(&sa), 4, host, sizeof(host), server, sizeof(server), 0);
    EXPECT_EQ("getnameinfo_0500", ret, FAMILY_NOTSUPPORTED);
}

/**
 * @tc.name      : getnameinfo_0600
 * @tc.desc      : Each parameter is valid and can resolve the IP address of the host name(hint.ai_flags =AI_ALL).
 * @tc.level     : Level 2
 */
void getnameinfo_0600()
{
    int ret = -1;
    int num = -1;
    int res = -1;
    struct addrinfo *result, hint;
    hint.ai_flags = AI_NUMERICHOST;
    hint.ai_family = AF_INET6;
    char buf[] = "fe80::bed5:4695:6cac:bef8";
    ret = getaddrinfo(buf, NULL, &hint, &result);
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];
    if (ret == 0) {
        res = getnameinfo(result->ai_addr, 4, host, sizeof(host), server, sizeof(server), 0);
    }
    EXPECT_EQ("getnameinfo_0600", res, FAMILY_NOTSUPPORTED);
    freeaddrinfo(result);
    result = NULL;
}

/**
 * @tc.name      : getnameinfo_0700
 * @tc.desc      : Each parameter is valid, sa->sa_family is AF_LOCAL, the information of the host cannot be obtained.
 * @tc.level     : Level 2
 */
void getnameinfo_0700()
{
    int ret;
    char host[NI_MAXHOST];
    char server[NI_MAXSERV];
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_LOCAL;
    ret = getnameinfo((const struct sockaddr *)(&sa), sizeof(sa), host, sizeof(host), server, sizeof(server), 0);
    EXPECT_EQ("getnameinfo_0700", ret, FAMILY_NOTSUPPORTED);
}

void getnameinfo_0900()
{
    int ret;
    char *ptr = "127.0.0.1";
    char hostname[128] = {0};
    char servername[128] = {0};
    struct sockaddr_in addrDst;

    memset(&addrDst, 0, sizeof(addrDst));
    addrDst.sin_family = AF_INET;
    addrDst.sin_addr.s_addr =  inet_addr(ptr);
    addrDst.sin_port = htons(HTTP_PORT);

    ret = getnameinfo((struct sockaddr *)&addrDst, sizeof(addrDst), hostname, sizeof(hostname),
     servername, sizeof(servername), 0);
    EXPECT_EQ("getnameinfo_0900", ret, 0);
    EXPECT_STREQ("getnameinfo_0900 hostname", hostname, "localhost");
    EXPECT_STREQ("getnameinfo_0900 hostname", servername, "http");
}

void getnameinfo_1000()
{
    int ret;
    char *ptr = "127.0.0.1";
    char hostname[128] = {0};
    char servername[128] = {0};
    struct sockaddr_in addrDst;

    memset(&addrDst, 0, sizeof(addrDst));
    addrDst.sin_family = AF_INET;
    addrDst.sin_addr.s_addr =  inet_addr(ptr);
    addrDst.sin_port = htons(TCPMUX_PORT);

    ret = getnameinfo((struct sockaddr *)&addrDst, sizeof(addrDst), hostname, sizeof(hostname),
     servername, sizeof(servername), 0);
    EXPECT_EQ("getnameinfo_1000", ret, 0);
    EXPECT_STREQ("getnameinfo_1000 hostname", hostname, "localhost");
    EXPECT_STREQ("getnameinfo_1000 hostname", servername, "tcpmux");
}


void getnameinfo_1100()
{
    int ret;
    char *ptr = "127.0.0.1";
    char hostname[128] = {0};
    char servername[128] = {0};
    struct sockaddr_in addrDst;

    memset(&addrDst, 0, sizeof(addrDst));
    addrDst.sin_family = AF_INET;
    addrDst.sin_addr.s_addr =  inet_addr(ptr);
    addrDst.sin_port = htons(FIDO_PORT);

    ret = getnameinfo((struct sockaddr *)&addrDst, sizeof(addrDst), hostname, sizeof(hostname),
     servername, sizeof(servername), 0);
    EXPECT_EQ("getnameinfo_1100", ret, 0);
    EXPECT_STREQ("getnameinfo_1100 hostname", hostname, "localhost");
    EXPECT_STREQ("getnameinfo_1100 hostname", servername, "fido");
}

void getnameinfo_1200()
{
    int ret;
    char *ptr = "127.0.0.1";
    char hostname[128] = {0};
    char servername[128] = {0};
    struct sockaddr_in addrDst;

    memset(&addrDst, 0, sizeof(addrDst));
    addrDst.sin_family = AF_INET;
    addrDst.sin_addr.s_addr =  inet_addr(ptr);
    addrDst.sin_port = htons(NOT_EXIST_PORT);

    ret = getnameinfo((struct sockaddr *)&addrDst, sizeof(addrDst), hostname, sizeof(hostname),
     servername, sizeof(servername), 0);
    EXPECT_EQ("getnameinfo_1200", ret, 0);
    EXPECT_STREQ("getnameinfo_1200 hostname", hostname, "localhost");
    EXPECT_STREQ("getnameinfo_1200 hostname", servername, NOT_EXIST_PORT_STR);
}

int main(int argc, char *argv[])
{
    getnameinfo_0100();
    getnameinfo_0200();
    getnameinfo_0300();
    getnameinfo_0400();
    getnameinfo_0500();
    getnameinfo_0600();
    getnameinfo_0700();
    getnameinfo_0900();
    getnameinfo_1000();
    getnameinfo_1100();
    getnameinfo_1200();
    return t_status;
}

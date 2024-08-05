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

#include <arpa/inet.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/*
 * @tc.name      : inet_ntop_0100
 * @tc.desc      : Verify that the data format is converted to IP address format (the address cluster is AF_INET).
 * @tc.level     : Level 0
 */
void inet_ntop_0100(void)
{
    char ip4test[] = "10.10.0.1";
    struct in_addr sin_addr;
    inet_pton(AF_INET, ip4test, &sin_addr);
    const char *ptr = inet_ntop(AF_INET, &sin_addr, ip4test, sizeof(ip4test));
    EXPECT_PTRNE("inet_ntop_0100", ptr, NULL);
    EXPECT_STREQ("inet_ntop_0100", ptr, ip4test);
}

/*
 * @tc.name      : inet_ntop_0200
 * @tc.desc      : Verify that the data format is converted to the IP address format
 *                 (the address cluster is AF_INET6, and the metadata is colon and hexadecimal notation)
 * @tc.level     : Level 0
 */
void inet_ntop_0200(void)
{
    char ip6test[] = "A157:CD01:3579:1526:DBAC:EF21:4356:7879";
    struct in_addr sin_addr;
    inet_pton(AF_INET6, ip6test, &sin_addr);
    const char *ptr = inet_ntop(AF_INET6, &sin_addr, ip6test, sizeof(ip6test));
    EXPECT_PTRNE("inet_ntop_0200", ptr, NULL);
    EXPECT_STREQ("inet_ntop_0200", ptr, ip6test);
}

/*
 * @tc.name      : inet_ntop_0300
 * @tc.desc      : Verify data format is converted to IP address format
 *                 (address cluster is AF_INET6, metadata is 0-bit compressed notation)
 * @tc.level     : Level 0
 */
void inet_ntop_0300(void)
{
    char ip6test[] = "fe80::bed5:4695:6cac:bef8";
    struct in_addr sin_addr;
    inet_pton(AF_INET6, ip6test, &sin_addr);
    const char *ptr = inet_ntop(AF_INET6, &sin_addr, ip6test, sizeof(ip6test));
    EXPECT_PTRNE("inet_ntop_0300", ptr, NULL);
    EXPECT_STREQ("inet_ntop_0300", ptr, ip6test);
}

/*
 * @tc.name      : inet_ntop_0400
 * @tc.desc      : Verify that the data format is converted to IP address format
 *                 (the address cluster is AF_INET6, and the metadata is the embedded Ipv address notation)
 * @tc.level     : Level 0
 */
void inet_ntop_0400(void)
{
    char ip6test[] = "::10.10.0.1";
    struct in_addr sin_addr;
    inet_pton(AF_INET6, ip6test, &sin_addr);
    const char *ptr = inet_ntop(AF_INET6, &sin_addr, ip6test, sizeof(ip6test));
    EXPECT_PTRNE("inet_ntop_0400", ptr, NULL);
    EXPECT_EQ("inet_ntop_0400", ptr, ip6test);
}

/*
 * @tc.name      : inet_ntop_0500
 * @tc.desc      : Verify that the data format is converted to IP address format (the address cluster is AF_UNIX)
 * @tc.level     : Level 2
 */
void inet_ntop_0500(void)
{
    char ip4test[] = "10.10.0.1";
    struct in_addr sin_addr;
    inet_pton(AF_UNIX, ip4test, &sin_addr);
    const char *ptr = inet_ntop(AF_UNIX, &sin_addr, ip4test, sizeof(ip4test));
    EXPECT_PTREQ("inet_ntop_0500", ptr, NULL);
}

/*
 * @tc.name      : inet_ntop_0600
 * @tc.desc      : Verify that the data format is converted to IP address format (the address cluster is AF_INET6)
 * @tc.level     : Level 2
 */
void inet_ntop_0600(void)
{
    char ip6test[] = "10.10.0.1";
    struct in_addr sin_addr;
    inet_pton(AF_INET6, ip6test, &sin_addr);
    const char *ptr = inet_ntop(AF_INET6, &sin_addr, ip6test, sizeof(ip6test));
    EXPECT_PTREQ("inet_ntop_0600", ptr, NULL);
}

TEST_FUN G_Fun_Array[] = {
    inet_ntop_0100,
    inet_ntop_0200,
    inet_ntop_0300,
    inet_ntop_0400,
    inet_ntop_0500,
    inet_ntop_0600,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
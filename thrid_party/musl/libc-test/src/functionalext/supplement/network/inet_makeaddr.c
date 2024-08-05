/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <arpa/inet.h>
#include <netinet/in.h>
#include "functionalext.h"

const uint32_t NET = 0x0000007f;
const uint32_t HOST = 0x00000001;
const uint32_t IP = 0x7f000001;

/**
 * @tc.name      : inet_makeaddr_0100
 * @tc.desc      : Verify inet_makeaddr process success
 * @tc.level     : Level 0
 */
void inet_makeaddr_0100(void)
{
    struct in_addr a = inet_makeaddr(NET, HOST);
    EXPECT_EQ("inet_makeaddr_0100", htonl(IP), a.s_addr);
}

/**
 * @tc.name      : inet_makeaddr_0200
 * @tc.desc      : Test the function of inet_makeaddr, n is 512.(n < 65536)
 * @tc.level     : Level 0
 */
void inet_makeaddr_0200(void)
{
    const uint32_t net = 0x00000200;
    const uint32_t ip = 0x02000001;
    struct in_addr a = inet_makeaddr(net, HOST);
    EXPECT_EQ("inet_makeaddr_0200", htonl(ip), a.s_addr);
}

/**
 * @tc.name      : inet_makeaddr_0300
 * @tc.desc      : Test the function of inet_makeaddr, n is 131072.(n > 65536)
 * @tc.level     : Level 0
 */
void inet_makeaddr_0300(void)
{
    const uint32_t net = 0x00020000;
    const uint32_t ip = 0x02000001;
    struct in_addr a = inet_makeaddr(net, HOST);
    EXPECT_EQ("inet_makeaddr_0200", htonl(ip), a.s_addr);
}

int main(void)
{
    inet_makeaddr_0100();
    inet_makeaddr_0200();
    inet_makeaddr_0300();
    return t_status;
}

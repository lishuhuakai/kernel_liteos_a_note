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
const uint32_t IP = 0x7f000001;

/**
 * @tc.name      : inet_netof_0100
 * @tc.desc      : Verify inet_netof process success
 * @tc.level     : Level 0
 */
void inet_netof_0100(void)
{
    struct in_addr a = {htonl(IP)};
    EXPECT_EQ("inet_netof_0100", inet_netof(a), NET);
}

/**
 * @tc.name      : inet_netof_0100
 * @tc.desc      : Test the function of inet_netof, n is 131072.(n > 65536)
 * @tc.level     : Level 0
 */
void inet_netof_0200(void)
{
    const uint32_t net =0x00008000;
    struct in_addr a = {htonl(0x80000001)};
    EXPECT_EQ("inet_netof_0100", inet_netof(a), net);
}

int main(void)
{
    inet_netof_0100();
    inet_netof_0200();
    return t_status;
}

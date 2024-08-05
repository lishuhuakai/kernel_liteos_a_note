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
#include "test.h"

/**
 * @tc.name      : inet_aton_0100
 * @tc.desc      : The format of the parameter _cp is a.b.c.d
 * @tc.level     : Level 0
 */
void inet_aton_0100()
{
    struct in_addr a;
    a.s_addr = 0;
    int ret = inet_aton("127.1.2.3", &a);
    if (ret != 1) {
        t_error("%s inet_aton failed\n", __func__);
    }
    if (a.s_addr != (htonl)(0x7f010203)) {
        t_error("%s inet_aton invalid\n", __func__);
    }
}

/**
 * @tc.name      : inet_aton_0200
 * @tc.desc      : The format of the parameter _cp is a.b.c
 * @tc.level     : Level 0
 */
void inet_aton_0200()
{
    struct in_addr a;
    a.s_addr = 0;
    int ret = inet_aton("127.1.2", &a);
    if (ret != 1) {
        t_error("%s inet_aton failed\n", __func__);
    }
    if (a.s_addr != (htonl)(0x7f010002)) {
        t_error("%s inet_aton invalid\n", __func__);
    }
}

/**
 * @tc.name      : inet_aton_0300
 * @tc.desc      : The format of the parameter _cp is a.b
 * @tc.level     : Level 0
 */
void inet_aton_0300()
{
    struct in_addr a;
    a.s_addr = 0;
    int ret = inet_aton("127.1", &a);
    if (ret != 1) {
        t_error("%s inet_aton failed\n", __func__);
    }
    if (a.s_addr != (htonl)(0x7f000001)) {
        t_error("%s inet_aton invalid\n", __func__);
    }
}

/**
 * @tc.name      : inet_aton_0400
 * @tc.desc      : The format of the parameter _cp is a
 * @tc.level     : Level 0
 */
void inet_aton_0400()
{
    struct in_addr a;
    a.s_addr = 0;
    int ret = inet_aton("0x7f000001", &a);
    if (ret != 1) {
        t_error("%s inet_aton failed\n", __func__);
    }
    if (a.s_addr != (htonl)(0x7f000001)) {
        t_error("%s inet_aton invalid\n", __func__);
    }
}

/**
 * @tc.name      : inet_aton_0500
 * @tc.desc      : Hex (0x) and mixed-case hex digits.
 * @tc.level     : Level 0
 */
void inet_aton_0500()
{
    struct in_addr a;
    a.s_addr = 0;
    int ret = inet_aton("0xFf.0.0.1", &a);
    if (ret != 1) {
        t_error("%s inet_aton failed\n", __func__);
    }
    if (a.s_addr != (htonl)(0xff000001)) {
        t_error("%s inet_aton invalid\n", __func__);
    }
}

/**
 * @tc.name      : inet_aton_0600
 * @tc.desc      : When the parameter is invalid, test the return value of the function
 * @tc.level     : Level 2
 */
void inet_aton_0600()
{
    int ret = inet_aton("", NULL);
    if (ret != 0) {
        t_error("%s inet_aton failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    inet_aton_0100();
    inet_aton_0200();
    inet_aton_0300();
    inet_aton_0400();
    inet_aton_0500();
    inet_aton_0600();
    return t_status;
}
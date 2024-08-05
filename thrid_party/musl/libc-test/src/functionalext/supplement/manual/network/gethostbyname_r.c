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
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdbool.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : gethostbyname_r_0100
 * @tc.desc      : Verify that the host information can be obtained
 * @tc.level     : Level 0
 */
void gethostbyname_r_0100(void)
{
    bool flag = false;
    int result = 10;
    struct hostent h;
    char buf[1024] = "\0";
    size_t buflen;
    struct hostent *res = NULL;
    int err;
    result = gethostbyname_r("127.0.0.1", &h, buf, 1024, &res, &err);
    if (result == 0) {
        flag = true;
    }
    EXPECT_TRUE("gethostbyname_r_0100", flag);
    EXPECT_STREQ("gethostbyname_r_0100", h.h_name, "127.0.0.1");
}

/**
 * @tc.name      : gethostbyname_r_0200
 * @tc.desc      : The host information cannot be obtained
 * @tc.level     : Level 2
 */
void gethostbyname_r_0200(void)
{
    bool flag = false;
    int result = 10;
    struct hostent h;
    char buf[1024] = "\0";
    size_t buflen;
    struct hostent *res = NULL;
    int err;
    result = gethostbyname_r("127.0.w.1", &h, buf, 1024, &res, &err);
    if (result != 0) {
        flag = true;
    }
    EXPECT_TRUE("gethostbyname_r_0200", flag);
}

TEST_FUN G_Fun_Array[] = {
    gethostbyname_r_0100,
    gethostbyname_r_0200,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}
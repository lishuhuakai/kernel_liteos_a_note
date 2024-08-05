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

/**
 * @tc.name      : gethostbyname2_0100
 * @tc.desc      : Verify that host information can be obtained (all parameters are valid, fd is AF_INET)
 * @tc.level     : Level 0
 */
void gethostbyname2_0100(void)
{
    bool flag = false;
    struct hostent *hptr;
    hptr = gethostbyname2("127.0.0.1", AF_INET);
    if (hptr != NULL) {
        flag = true;
    }
    EXPECT_TRUE("gethostbyname2_0100", flag);
    EXPECT_STREQ("gethostbyname2_0100", hptr->h_name, "127.0.0.1");
}

/**
 * @tc.name      : gethostbyname2_0200
 * @tc.desc      : Verify that the host information can be obtained (all parameters are valid, fd is AF_INET6)
 * @tc.level     : Level 0
 */
void gethostbyname2_0200(void)
{
    bool flag = false;
    struct hostent *hptr;
    hptr = gethostbyname2("::1", AF_INET6);
    if (hptr != NULL) {
        flag = true;
    }
    EXPECT_TRUE("gethostbyname2_0200", flag);
    EXPECT_STREQ("gethostbyname2_0200", hptr->h_name, "::1");
}

/**
 * @tc.name      : gethostbyname2_0300
 * @tc.desc      : Verify cannot get host information (name parameter invalid)
 * @tc.level     : Level 2
 */
void gethostbyname2_0300(void)
{
    bool flag = false;
    struct hostent *hptr;
    hptr = gethostbyname2("127.0.w.1", AF_INET6);
    if (hptr == NULL) {
        flag = true;
    }
    EXPECT_TRUE("gethostbyname2_0300", flag);
}

/**
 * @tc.name      : gethostbyname2_0400
 * @tc.desc      : Verify that host information cannot be obtained (fd parameter invalid)
 * @tc.level     : Level 2
 */
void gethostbyname2_0400(void)
{
    bool flag = false;
    struct hostent *hptr;
    hptr = gethostbyname2("::1", AF_INET);
    if (hptr == NULL) {
        flag = true;
    }
    EXPECT_TRUE("gethostbyname2_0400", flag);
}

TEST_FUN G_Fun_Array[] = {
    gethostbyname2_0100,
    gethostbyname2_0200,
    gethostbyname2_0300,
    gethostbyname2_0400,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
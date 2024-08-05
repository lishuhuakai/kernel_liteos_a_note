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
#include <netinet/in.h>
#include <sys/socket.h>
#include "functionalext.h"

/**
 * @tc.name      : gethostbyname2_r_0100
 * @tc.desc      : The parameter fd is AF_INET, and multiple threads simultaneously obtain host information.
 * @tc.level     : Level 0
 */
void gethostbyname2_r_0100(void)
{
    struct hostent h, *res = NULL;
    char buf[8192] = {0};
    int err = 0;

    char *name = "127.0.0.1";
    int ret = gethostbyname2_r(name, AF_INET, &h, buf, sizeof(buf), &res, &err);
    EXPECT_EQ("gethostbyname2_r_0100", ret, 0);
    EXPECT_STREQ("gethostbyname2_r_0100", h.h_name, "127.0.0.1");
}

/**
 * @tc.name      : gethostbyname2_r_0200
 * @tc.desc      : The parameter fd is AF_INET6, and multiple threads simultaneously obtain host information.
 * @tc.level     : Level 0
 */
void gethostbyname2_r_0200(void)
{
    struct hostent h, *res = NULL;
    char buf[8192] = {0};
    int err = 0;

    char *name = "::1";
    int ret = gethostbyname2_r(name, AF_INET6, &h, buf, sizeof(buf), &res, &err);
    EXPECT_EQ("gethostbyname2_r_0200", ret, 0);
    EXPECT_TRUE("gethostbyname2_r_0200", strcmp(h.h_name, "::1") == 0);
}

/**
 * @tc.name      : gethostbyname2_r_0300
 * @tc.desc      : The parameter name is invalid, the host information cannot be obtained.
 * @tc.level     : Level 2
 */
void gethostbyname2_r_0300(void)
{
    struct hostent h, *res = NULL;
    char buf[8192] = {0};
    int err = 0;
    char *name = "aaaa";
    int ret = gethostbyname2_r(name, AF_INET, &h, buf, sizeof(buf), &res, &err);
    EXPECT_NE("gethostbyname2_r_0300", ret, 0);
}

/**
 * @tc.name      : gethostbyname2_r_0400
 * @tc.desc      : The parameter fd is invalid, the host information cannot be obtained.
 * @tc.level     : Level 2
 */
void gethostbyname2_r_0400(void)
{
    struct hostent h, *res = NULL;
    char buf[8192] = {0};
    int err = 0;
    char *name = "localhost";
    int ret = gethostbyname2_r(name, 0, &h, buf, sizeof(buf), &res, &err);
    EXPECT_EQ("gethostbyname2_r_0400", ret, 0);
    EXPECT_STREQ("gethostbyname2_r_0400", "localhost", h.h_name);
}

int main(int argc, char *argv[])
{
    gethostbyname2_r_0100();
    gethostbyname2_r_0200();
    gethostbyname2_r_0300();
    gethostbyname2_r_0400();
    return t_status;
}

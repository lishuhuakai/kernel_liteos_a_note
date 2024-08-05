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
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include "functionalext.h"

/**
 * @tc.name      : gethostbyaddr_r_0100
 * @tc.desc      : Each parameter is valid, and multiple threads simultaneously obtain host information.
 * @tc.level     : Level 0
 */
void gethostbyaddr_r_0100(void)
{
    struct hostent h, *res = NULL;
    char buf[8192] = {0};
    int err = 0;
    in_addr_t a = inet_addr("127.0.0.1");
    int ret = gethostbyaddr_r((void *)&a, 4, AF_INET, &h, buf, sizeof(buf), &res, &err);
    EXPECT_EQ("gethostbyaddr_r_0100", ret, 0);
    EXPECT_TRUE("gethostbyaddr_r_0100", strcmp(h.h_name, "localhost") == 0);
}

/**
 * @tc.name      : gethostbyaddr_r_0200
 * @tc.desc      : Invalid parameter, failed to get host information.
 * @tc.level     : Level 2
 */
void gethostbyaddr_r_0200(void)
{
    struct hostent h, *res = NULL;
    char buf[8192] = {0};
    int err = 0;
    in_addr_t a = inet_addr("127.0.0.1");
    int ret = gethostbyaddr_r((void *)&a, 0, AF_INET, &h, buf, sizeof(buf), &res, &err);
    EXPECT_NE("gethostbyaddr_r_0200", ret, 0);
}

int main(int argc, char *argv[])
{
    gethostbyaddr_r_0100();
    gethostbyaddr_r_0200();
    return t_status;
}
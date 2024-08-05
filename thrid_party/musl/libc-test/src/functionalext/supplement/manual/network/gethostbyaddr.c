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

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "functionalext.h"

/**
 * @tc.name      : gethostbyaddre_0100
 * @tc.desc      : Can get host information through an IPv4 address.
 * @tc.level     : Level 0
 */
void gethostbyaddre_0100(void)
{
    char *ptr = "127.0.0.1";
    struct hostent *hptr = NULL;
    struct in_addr addr;
    if (inet_pton(AF_INET, ptr, &addr) <= 0) {
        t_error("%s inet_pton error:%s\n", __func__, strerror(errno));
    }
    hptr = gethostbyaddr((const char *)&addr, sizeof(addr), AF_INET);
    if (hptr == NULL) {
        t_error("%s gethostbyaddr error:%s\n", __func__, strerror(h_errno));
    }
    EXPECT_TRUE("gethostbyaddre_0100", hptr != NULL);
    EXPECT_STREQ("gethostbyaddre_0100", hptr->h_name, "localhost");
}

/**
 * @tc.name      : gethostbyaddre_0200
 * @tc.desc      : Can obtain host information through an IPv6 address.
 * @tc.level     : Level 0
 */
void gethostbyaddre_0200(void)
{
    char *ptr = "fe80::bed5:4695:6cac:bef8";
    struct hostent *hptr = NULL;
    struct in_addr addr;
    addr.s_addr = inet_addr(ptr);
    hptr = gethostbyaddr((void *)&addr.s_addr, 16, AF_INET6);
    EXPECT_TRUE("gethostbyaddre_0200", hptr != NULL);
}

int main(int argc, char *argv[])
{
    gethostbyaddre_0100();
    gethostbyaddre_0200();
    return t_status;
}

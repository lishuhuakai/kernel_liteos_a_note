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

#include <netdb.h>
#include "functionalext.h"

#define TEST_PROTO_NUM 6
#define TEST_INVALID_NUMBER 10240
#define TEST_TERMINATE_SIZE 2

static int test_index = 0;
static const unsigned char test_protos[] = {"\000ip\0"
                                            "\001icmp\0"
                                            "\002igmp\0"
                                            "\003ggp\0"
                                            "\004ipencap\0"
                                            "\005st\0"
                                            "\006tcp\0"
                                            "\010egp\0"
                                            "\014pup\0"
                                            "\021udp\0"
                                            "\024hmp\0"
                                            "\026xns-idp\0"
                                            "\033rdp\0"
                                            "\035iso-tp4\0"
                                            "\044xtp\0"
                                            "\045ddp\0"
                                            "\046idpr-cmtp\0"
                                            "\051ipv6\0"
                                            "\053ipv6-route\0"
                                            "\054ipv6-frag\0"
                                            "\055idrp\0"
                                            "\056rsvp\0"
                                            "\057gre\0"
                                            "\062esp\0"
                                            "\063ah\0"
                                            "\071skip\0"
                                            "\072ipv6-icmp\0"
                                            "\073ipv6-nonxt\0"
                                            "\074ipv6-opts\0"
                                            "\111rspf\0"
                                            "\121vmtp\0"
                                            "\131ospf\0"
                                            "\136ipip\0"
                                            "\142encap\0"
                                            "\147pim\0"
                                            "\377raw"};

struct protoent *test_getprotoent(void)
{
    static struct protoent p;
    if (test_index >= sizeof(test_protos)) {
        return NULL;
    }
    p.p_proto = test_protos[test_index];
    p.p_name = (char *)&test_protos[test_index + 1];
    test_index += strlen(p.p_name) + TEST_TERMINATE_SIZE;
    return &p;
}

void test_resetprotoent(void)
{
    test_index = 0;
    setprotoent(0);
    endprotoent();
}

/**
 * @tc.name      : getprotobyname_0100
 * @tc.desc      : Get the specified protocol name by name
 * @tc.level     : Level 0
 */
void getprotobyname_0100(void)
{
    struct protoent *ret = getprotobyname("tcp");
    EXPECT_PTRNE("getprotobyname_0100", ret, NULL);
    if (ret) {
        EXPECT_STREQ("getprotobyname_0100", ret->p_name, "tcp");
        EXPECT_EQ("getprotobyname_0100", ret->p_proto, TEST_PROTO_NUM);
    }
}

/**
 * @tc.name      : getprotobyname_0200
 * @tc.desc      : Get the specified protocol name by name
 * @tc.level     : Level 2
 */
void getprotobyname_0200(void)
{
    struct protoent *ret = getprotobyname("abcd");
    EXPECT_PTREQ("getprotobyname_0200", ret, NULL);
}

/**
 * @tc.name      : getprotobynumber_0100
 * @tc.desc      : Get the specified protocol name by number
 * @tc.level     : Level 0
 */
void getprotobynumber_0100(void)
{
    struct protoent *ret = getprotobynumber(TEST_PROTO_NUM);
    EXPECT_PTRNE("getprotobynumber_0100", ret, NULL);
    if (ret) {
        EXPECT_STREQ("getprotobynumber_0100", ret->p_name, "tcp");
        EXPECT_EQ("getprotobynumber_0100", ret->p_proto, TEST_PROTO_NUM);
    }
}

/**
 * @tc.name      : getprotobynumber_0200
 * @tc.desc      : Get the specified protocol name by number
 * @tc.level     : Level 2
 */
void getprotobynumber_0200(void)
{
    struct protoent *ret = getprotobynumber(TEST_INVALID_NUMBER);
    EXPECT_PTREQ("getprotobynumber_0200", ret, NULL);
}

/**
 * @tc.name      : getprotoent_0100
 * @tc.desc      : Get the protocol information scheduled by the system
 * @tc.level     : Level 0
 */
void getprotoent_0100(void)
{
    test_resetprotoent();
    struct protoent *dst = NULL;
    struct protoent *src = NULL;
    int count = 0;
    while (1) {
        dst = getprotoent();
        src = test_getprotoent();
        if (dst && src) {
            count++;
            EXPECT_EQ("getprotoent_0100", dst->p_proto, src->p_proto);
            EXPECT_STREQ("getprotoent_0100", dst->p_name, src->p_name);
        } else {
            break;
        }
    }
    EXPECT_TRUE("getprotoent_0100", count > 0);
    test_resetprotoent();
}

int main(void)
{
    getprotobyname_0100();
    getprotobyname_0200();

    getprotobynumber_0100();
    getprotobynumber_0200();

    getprotoent_0100();
    return t_status;
}
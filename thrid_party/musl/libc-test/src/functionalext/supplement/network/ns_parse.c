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
#include <arpa/nameser.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "functionalext.h"

#define TEST_MSG_SIZE 8
#define TEST_DNS_HEAD 12
#define TEST_SHIFT 8
#define TEST_INDEX_2 2
#define TEST_INDEX_3 3
#define TEST_INDEX_5 5
#define TEST_SKIP_SIZE 4
#define TEST_DATA_LEN 10

struct test_skiprr_data {
    int sect;
    int result;
};

static unsigned char msg[] = "\x71\x79\x81\x80\x00\x01"
                             "\x00\x02\x00\x04\x00\x04\x03\x77\x77\x77\x03\x61\x62\x63\x03\x63"
                             "\x6f\x6d\x00\x00\x01\x00\x01\xc0\x0c\x00\x05\x00\x01\x00\x00\x02"
                             "\xe8\x00\x02\xc0\x10\xc0\x10\x00\x01\x00\x01\x00\x00\x02\xe9\x00"
                             "\x04\x0a\xb5\x84\xfa\xc0\x10\x00\x02\x00\x01\x00\x00\xda\xeb\x00"
                             "\x0d\x06\x73\x65\x6e\x73\x30\x31\x03\x64\x69\x67\xc0\x14\xc0\x10"
                             "\x00\x02\x00\x01\x00\x00\xda\xeb\x00\x09\x06\x73\x65\x6e\x73\x30"
                             "\x32\xc0\x4e\xc0\x10\x00\x02\x00\x01\x00\x00\xda\xeb\x00\x09\x06"
                             "\x6f\x72\x6e\x73\x30\x31\xc0\x4e\xc0\x10\x00\x02\x00\x01\x00\x00"
                             "\xda\xeb\x00\x09\x06\x6f\x72\x6e\x73\x30\x32\xc0\x4e\xc0\x75\x00"
                             "\x01\x00\x01\x00\x00\x7a\x36\x00\x04\x0a\xbb\xbd\x2c\xc0\x8a\x00"
                             "\x01\x00\x01\x00\x00\x1b\x96\x00\x04\x0a\xbb\xbe\x2c\xc0\x47\x00"
                             "\x01\x00\x01\x00\x00\x92\xb1\x00\x04\x0a\xb5\x86\x10\xc0\x60\x00"
                             "\x01\x00\x01\x00\x00\x92\xb1\x00\x04\x0a\xb5\x87\xc7";

static struct test_skiprr_data test_data[TEST_SKIP_SIZE] = {{1, 17}, {2, 30}, {4, 88}, {4, 64}};

/**
 * @tc.name      : ns_get16_0100
 * @tc.desc      : Parse data from array and return
 * @tc.level     : Level 0
 */
void ns_get16_0100(void)
{
    const unsigned char cp[] = "11";
    unsigned int ret = ns_get16(cp);
    unsigned int getresult = 12593;
    EXPECT_EQ("ns_get16_0100", ret, getresult);
}

/**
 * @tc.name      : ns_get32_0100
 * @tc.desc      : Parse data from array and return
 * @tc.level     : Level 0
 */
void ns_get32_0100(void)
{
    const unsigned char cp[] = "11";
    unsigned long ret = ns_get32(cp);
    unsigned long getresult = 825294897;
    EXPECT_LONGEQ("ns_get32_0100", ret, getresult);
}

/**
 * @tc.name      : ns_put16_0100
 * @tc.desc      : Get actual data value from data of ns_get16
 * @tc.level     : Level 0
 */
void ns_put16_0100(void)
{
    const unsigned char from[] = "11";
    unsigned char to[TEST_MSG_SIZE];
    memset(to, 0x0, TEST_MSG_SIZE);
    unsigned int ret = ns_get16(from);
    ns_put16(ret, to);
    for (int i = 0; i < sizeof(from) / sizeof(unsigned char); i++) {
        EXPECT_EQ("ns_put16_0100", from[i], to[i]);
    }
}

/**
 * @tc.name      : ns_put32_0100
 * @tc.desc      : Get actual data value from data of ns_get32
 * @tc.level     : Level 0
 */
void ns_put32_0100(void)
{
    const unsigned char from[] = "1234";
    unsigned char to[TEST_MSG_SIZE];
    memset(to, 0x0, TEST_MSG_SIZE);
    unsigned long ret = ns_get32(from);
    ns_put32(ret, to);
    for (int i = 0; i < sizeof(from) / sizeof(unsigned char); i++) {
        EXPECT_LONGEQ("ns_put32_0100", from[i], to[i]);
    }
}

/**
 * @tc.name      : ns_name_uncompress_0100
 * @tc.desc      : Get the domain name from the array and store the data in the array (success)
 * @tc.level     : Level 0
 */
void ns_name_uncompress_0100(void)
{
    unsigned char domain[] = {2, 'p', 'q', 0xc0, 5, 0};
    char name[] = "XXXX";

    int ret = ns_name_uncompress(domain, domain + 6, domain, name, TEST_MSG_SIZE);
    EXPECT_EQ("ns_name_uncompress_0100", ret, 5);
    EXPECT_STREQ("ns_name_uncompress_0100", name, "pq");

    memcpy(domain, "\xc0\x02", 3);
    memcpy(name, "XXXX", 5);
    size_t limitsize = 1;
    ret = ns_name_uncompress(domain, domain + 3, domain, name, limitsize);

    EXPECT_EQ("ns_name_uncompress_0100", ret, 2);
    EXPECT_STREQ("ns_name_uncompress_0100", name, "");
}

/**
 * @tc.name      : ns_name_uncompress_0200
 * @tc.desc      : Get the domain name from the array and store the data in the array (failed)
 * @tc.level     : Level 2
 */
void ns_name_uncompress_0200(void)
{
    unsigned char domain[] = {2, 'p', 'q', 0xc0, 5, 0};
    char name[] = "XXXX";

    int ret = ns_name_uncompress(domain, domain + 6, domain, name, ERREXPECT);
    EXPECT_EQ("ns_name_uncompress_0200", ret, ERREXPECT);
    EXPECT_EQ("ns_name_uncompress_0200", errno, EMSGSIZE);
}

/**
 * @tc.name      : ns_name_uncompress_0300
 * @tc.desc      : Get the domain name from the array and store the data in the array (failed)
 * @tc.level     : Level 2
 */
void ns_name_uncompress_0300(void)
{
    unsigned char domain[] = "error value";
    char name[] = "XXXX";

    int ret = ns_name_uncompress(domain, domain + 6, domain, name, ERREXPECT);
    EXPECT_EQ("ns_name_uncompress_0200", ret, ERREXPECT);
    EXPECT_EQ("ns_name_uncompress_0200", errno, EMSGSIZE);
}

/**
 * @tc.name      : ns_initparse_0100
 * @tc.desc      : Parse DNS protocol reply packets into ns_msg structure
 * @tc.level     : Level 0
 */
void ns_initparse_0100(void)
{
    ns_msg handle;
    errno = 0;
    int ret = ns_initparse(msg, sizeof(msg) - 1, &handle);
    EXPECT_EQ("ns_initparse_0100", ret, CMPFLAG);
    EXPECT_EQ("ns_initparse_0100", errno, CMPFLAG);

    uint16_t id = (msg[0] << TEST_SHIFT) | msg[1];
    uint16_t flags = (msg[TEST_INDEX_2] << TEST_SHIFT) | msg[TEST_INDEX_3];
    EXPECT_EQ("ns_initparse_0100", handle._id, id);
    EXPECT_EQ("ns_initparse_0100", handle._flags, flags);
}

/**
 * @tc.name      : ns_initparse_0200
 * @tc.desc      : Provide error parameter data, parse DNS protocol response packet into ns_msg structure
 * @tc.level     : Level 2
 */
void ns_initparse_0200(void)
{
    ns_msg handle;
    errno = 0;
    int ret = ns_initparse(msg, TEST_DATA_LEN, &handle);
    EXPECT_EQ("ns_initparse_0200", ret, ERREXPECT);
    EXPECT_EQ("ns_initparse_0200", errno, EMSGSIZE);

    errno = 0;
    ret = ns_initparse(msg, sizeof(msg), &handle);
    EXPECT_EQ("ns_initparse_0200", ret, ERREXPECT);
    EXPECT_EQ("ns_initparse_0200", errno, EMSGSIZE);
}

/**
 * @tc.name      : ns_skiprr_0100
 * @tc.desc      : Find the record location from a given DNS packet
 * @tc.level     : Level 0
 */
void ns_skiprr_0100(void)
{
    unsigned char *ptr = msg + TEST_DNS_HEAD;
    unsigned char *eom = msg + sizeof(msg) - 1;
    int i, ret;
    for (i = 0; i < TEST_SKIP_SIZE; i++) {
        errno = 0;
        ret = ns_skiprr(ptr, eom, i, test_data[i].sect);
        if (ret == -1) {
            break;
        }
        ptr += ret;
        EXPECT_EQ("ns_skiprr_0100", ret, test_data[i].result);
        EXPECT_EQ("ns_skiprr_0100", errno, CMPFLAG);
    }
    EXPECT_PTREQ("ns_skiprr_0100", ptr, eom);
}

/**
 * @tc.name      : ns_skiprr_0200
 * @tc.desc      : Provide error message, find record location
 * @tc.level     : Level 2
 */
void ns_skiprr_0200(void)
{
    unsigned char *ptr = msg;
    unsigned char *eom = msg + sizeof(msg);
    int i, ret;
    for (i = 0; i < TEST_SKIP_SIZE; i++) {
        errno = 0;
        ret = ns_skiprr(ptr, eom, i, test_data[i].sect);
        if (ret == -1) {
            break;
        }
        ptr += ret;
    }
    EXPECT_PTRNE("ns_skiprr_0200", ptr, eom);
}

/**
 * @tc.name      : ns_parserr_0100
 * @tc.desc      : Extract data from a given DNS packet
 * @tc.level     : Level 0
 */
void ns_parserr_0100(void)
{
    ns_msg handle;
    errno = 0;
    int ret = ns_initparse(msg, sizeof(msg) - 1, &handle);
    EXPECT_EQ("ns_parserr_0100", ret, 0);
    if (ret != 0) {
        return;
    }

    ns_rr rr;
    memset(&rr, 0x0, sizeof(ns_rr));
    ret = ns_parserr(&handle, ns_s_qd, 0, &rr);
    EXPECT_EQ("ns_parserr_0100", ret, 0);
    EXPECT_STREQ("ns_parserr_0100", rr.name, "www.abc.com");
    EXPECT_EQ("ns_parserr_0100", rr.rr_class, 1);
    EXPECT_EQ("ns_parserr_0100", rr.type, 1);
    EXPECT_EQ("ns_parserr_0100", rr.ttl, 0);
    EXPECT_EQ("ns_parserr_0100", rr.rdlength, 0);
    EXPECT_PTREQ("ns_parserr_0100", rr.rdata, NULL);

    memset(&rr, 0x0, sizeof(ns_rr));
    ret = ns_parserr(&handle, ns_s_an, 0, &rr);
    EXPECT_EQ("ns_parserr_0100", ret, 0);
    EXPECT_STREQ("ns_parserr_0100", rr.name, "www.abc.com");
    EXPECT_EQ("ns_parserr_0100", rr.rr_class, 1);
    EXPECT_EQ("ns_parserr_0100", rr.type, TEST_INDEX_5);
    EXPECT_TRUE("ns_parserr_0100", rr.ttl > 0);
    EXPECT_TRUE("ns_parserr_0100", rr.rdlength > 0);
    EXPECT_PTRNE("ns_parserr_0100", rr.rdata, NULL);
}

/**
 * @tc.name      : ns_parserr_0200
 * @tc.desc      : Extracting data from given DNS message by providing wrong message length
 * @tc.level     : Level 2
 */
void ns_parserr_0200(void)
{
    ns_msg handle;
    errno = 0;
    int ret = ns_initparse(msg, sizeof(msg) - 1, &handle);
    EXPECT_EQ("ns_parserr_0200", ret, 0);
    if (ret != 0) {
        return;
    }

    ns_rr rr;
    errno = 0;
    memset(&rr, 0x0, sizeof(ns_rr));
    ret = ns_parserr(&handle, -1, 0, &rr);
    EXPECT_EQ("ns_parserr_0200", ret, -1);
    EXPECT_EQ("ns_parserr_0200", errno, ENODEV);

    errno = 0;
    memset(&rr, 0x0, sizeof(ns_rr));
    ret = ns_parserr(&handle, ns_s_max + 1, 0, &rr);
    EXPECT_EQ("ns_parserr_0200", ret, -1);
    EXPECT_EQ("ns_parserr_0200", errno, ENODEV);
}

/**
 * @tc.name      : ns_parserr_0300
 * @tc.desc      : Provides an uninitialized ns_msg to extract data from the given DNS message
 * @tc.level     : Level 2
 */
void ns_parserr_0300(void)
{
    ns_msg handle;
    errno = 0;
    ns_rr rr;
    memset(&handle, 0x0, sizeof(ns_msg));
    memset(&rr, 0x0, sizeof(ns_rr));

    int ret = ns_parserr(&handle, ns_s_qd, 0, &rr);
    EXPECT_EQ("ns_parserr_0300", ret, -1);
    EXPECT_EQ("ns_parserr_0300", errno, ENODEV);
}

int main(void)
{
    ns_get16_0100();
    ns_get32_0100();
    ns_put16_0100();
    ns_put32_0100();
    ns_name_uncompress_0100();
    ns_name_uncompress_0200();
    ns_name_uncompress_0300();
    ns_initparse_0100();
    ns_initparse_0200();
    ns_skiprr_0100();
    ns_skiprr_0200();
    ns_parserr_0100();
    ns_parserr_0200();
    ns_parserr_0300();
    return t_status;
}
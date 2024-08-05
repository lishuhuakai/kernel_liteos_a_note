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

#include <resolv.h>
#include "functionalext.h"

/**
 * @tc.name      : dn_comp_0100
 * @tc.desc      : Verify domain name compression (all parameters are valid)
 * @tc.level     : Level 0
 */
void dn_comp_0100(void)
{
    char name[1024] = "www.baidu.com";
    unsigned char buf[1024] = "\0";
    int result = dn_comp(name, buf, 1024, NULL, NULL);
    EXPECT_EQ("dn_comp_0100", result, 15);
}

/**
 * @tc.name      : dn_comp_0200
 * @tc.desc      : Verify that the domain name cannot be compressed (SRC parameter invalid, length 1)
 * @tc.level     : Level 1
 */
void dn_comp_0200(void)
{
    char name[1024] = ".";
    unsigned char buf[1024] = "\0";
    int result = dn_comp(name, buf, 1024, NULL, NULL);
    EXPECT_EQ("dn_comp_0200", result, 1);
}

/**
 * @tc.name      : dn_comp_0300
 * @tc.desc      : Verify that the domain name cannot be compressed (SRC parameter invalid, length greater than 253)
 * @tc.level     : Level 2
 */
void dn_comp_0300(void)
{
    char name[1024];
    for (int i = 0; i < 260; i++) {
        name[i] = 'w';
    }
    unsigned char buf[1024] = "\0";
    int result = dn_comp(name, buf, 1024, NULL, NULL);
    EXPECT_EQ("dn_comp_0300", result, -1);
}

/**
 * @tc.name      : dn_comp_0400
 * @tc.desc      : Verify that the domain name cannot be compressed (the space parameter is invalid, equal to 0)
 * @tc.level     : Level 2
 */
void dn_comp_0400(void)
{
    char name[1024] = "www.baidu.com";
    unsigned char buf[1024] = "\0";
    int result = dn_comp(name, buf, 0, NULL, NULL);
    EXPECT_EQ("dn_comp_0400", result, -1);
}

/**
 * @tc.name      : dn_comp_0500
 * @tc.desc      : Verify that the domain name cannot be compressed (space argument is invalid, less than 0)
 * @tc.level     : Level 2
 */
void dn_comp_0500(void)
{
    char name[1024] = "www.baidu.com";
    unsigned char buf[1024] = "\0";
    int result = dn_comp(name, buf, -1, NULL, NULL);
    EXPECT_EQ("dn_comp_0500", result, -1);
}

/**
 * @tc.name      : dn_comp_0600
 * @tc.desc      : Verify that the domain name cannot be compressed (space argument is invalid, equal to 1)
 * @tc.level     : Level 2
 */
void dn_comp_0600(void)
{
    char name[1024] = "www.baidu.com";
    unsigned char buf[1024] = "\0";
    int result = dn_comp(name, buf, 1, NULL, NULL);
    EXPECT_EQ("dn_comp_0600", result, -1);
}

int main(int argc, char *argv[])
{
    dn_comp_0100();
    dn_comp_0200();
    dn_comp_0300();
    dn_comp_0400();
    dn_comp_0500();
    dn_comp_0600();
    return t_status;
}
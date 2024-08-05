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
 * @tc.name      : dn_skipname_0100
 * @tc.desc      : Verify that the domain name can be compressed (parameters are valid)
 * @tc.level     : Level 0
 */
void dn_skipname_0100(void)
{
    unsigned char s[10] = {0, 2, 34, 45, 75, 23, 12, 192, 87, 254};
    int result = dn_skipname(&s[0], &s[1]);
    EXPECT_EQ("dn_skipname_0100", result, 1);
}

/**
 * @tc.name      : dn_skipname_0200
 * @tc.desc      : Verify that the domain name can be compressed (parameters are valid)
 * @tc.level     : Level 1
 */
void dn_skipname_0200(void)
{
    unsigned char s[10] = {0, 2, 34, 45, 75, 23, 12, 192, 87, 254};
    int result = dn_skipname(&s[7], &s[9]);
    EXPECT_EQ("dn_skipname_0200", result, 2);
}

/**
 * @tc.name      : dn_skipname_0300
 * @tc.desc      : Verify that the domain name cannot be compressed (s and end are equal)
 * @tc.level     : Level 2
 */
void dn_skipname_0300(void)
{
    unsigned char s[10] = {0, 2, 2, 45, 75, 23, 12, 192, 87, 254};
    int result = dn_skipname(&s[1], &s[2]);
    EXPECT_EQ("dn_skipname_0300", result, -1);
}

int main(int argc, char *argv[])
{
    dn_skipname_0100();
    dn_skipname_0200();
    dn_skipname_0300();

    return t_status;
}
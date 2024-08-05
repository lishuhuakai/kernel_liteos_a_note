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

#include <ctype.h>
#include "functionalext.h"

/**
 * @tc.name      : isgraph_0100
 * @tc.desc      : Whether the characters in a string are printable characters
 * @tc.level     : Level 0
 */
void isgraph_0100(void)
{
    const char *str = "1qw@#";
    const char *p = str;
    while (*p++ && *p != '\0') {
        int ret = isgraph(*p);
        EXPECT_EQ("isgraph_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : isgraph_0200
 * @tc.desc      : Whether the characters in a string are none-printable characters
 * @tc.level     : Level 2
 */
void isgraph_0200(void)
{
    const char *str = " ";
    const char *p = str;
    while (*p++ && *p != '\0') {
        int ret = isgraph(*p);
        EXPECT_EQ("isgraph_0200", ret, CMPFLAG);
    }
}

int main(void)
{
    isgraph_0100();
    isgraph_0200();
    return t_status;
}
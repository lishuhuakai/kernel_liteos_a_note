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

const int COUNT = 22;
const int EOK = 0;

/**
 * @tc.name      : isxdigit_0100
 * @tc.desc      : The parameter c is alphanumeric, and the input character is judged to be a hexadecimal number.
 * @tc.level     : Level 0
 */
void isxdigit_0100(void)
{
    int ret = isxdigit('0');
    EXPECT_NE("isxdigit_0100", ret, EOK);
}

/**
 * @tc.name      : isxdigit_0200
 * @tc.desc      : The parameter c is a valid English letter(uppercase),
 *                 and the input character is judged to be a hexadecimal number.
 * @tc.level     : Level 0
 */
void isxdigit_0200(void)
{
    int ret = isxdigit('F');
    EXPECT_NE("isxdigit_0200", ret, EOK);
}

/**
 * @tc.name      : isxdigit_0300
 * @tc.desc      : The parameter c is a valid English letter(lowercase),
 *                 and the input character is judged to be a hexadecimal number.
 * @tc.level     : Level 0
 */
void isxdigit_0300(void)
{
    int ret = isxdigit('a');
    EXPECT_NE("isxdigit_0300", ret, EOK);
}

/**
 * @tc.name      : isxdigit_0400
 * @tc.desc      : The parameter c is a invalid English letter(uppercase),
 *                 and the input character is not a hexadecimal number.
 * @tc.level     : Level 2
 */
void isxdigit_0400(void)
{
    int ret = isxdigit('G');
    EXPECT_EQ("isxdigit_0400", ret, EOK);
}

/**
 * @tc.name      : isxdigit_0500
 * @tc.desc      : The parameter c is a special letter(%),
 *                 and the input character is not a hexadecimal number.
 * @tc.level     : Level 2
 */
void isxdigit_0500(void)
{
    int ret = isxdigit('%');
    EXPECT_EQ("isxdigit_0500", ret, EOK);
}

/**
 * @tc.name      : isxdigit_0600
 * @tc.desc      : Determine the number of hexadecimal letters in the ascii code table.
 * @tc.level     : Level 2
 */
void isxdigit_0600(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isxdigit((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isxdigit_0600", total, COUNT);
}

/**
 * @tc.name      : isxdigit_l_0100
 * @tc.desc      : Whether the characters in a string is hexadecimal digit character
 * @tc.level     : Level 0
 */
void isxdigit_l_0100(void)
{
    const char *str = "0123456789AaBbCcDdEeFf";
    const char *p = str;
    while (*p++ && *p != '\0') {
        int ret = isxdigit_l(*p, NULL);
        EXPECT_EQ("isxdigit_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : isxdigit_l_0200
 * @tc.desc      : Whether the characters in a string is non-hexadecimal digit character
 * @tc.level     : Level 2
 */
void isxdigit_l_0200(void)
{
    const char *str = "!@hHiIjJZz";
    const char *p = str;
    while (*p++ && *p != '\0') {
        int ret = isxdigit_l(*p, NULL);
        EXPECT_EQ("isxdigit_l_0200", ret, CMPFLAG);
    }
}

int main(void)
{
    isxdigit_0100();
    isxdigit_0200();
    isxdigit_0300();
    isxdigit_0400();
    isxdigit_0500();
    isxdigit_0600();
    isxdigit_l_0100();
    isxdigit_l_0200();
    return t_status;
}
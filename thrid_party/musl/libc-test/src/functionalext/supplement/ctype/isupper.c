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

const int COUNT = 26;
const int EOK = 0;

/**
 * @tc.name      : isupper_0100
 * @tc.desc      : The parameter c is a uppercase letter,which determines the case of the input character.
 * @tc.level     : Level 0
 */
void isupper_0100(void)
{
    int ret = isupper('A');
    EXPECT_NE("isupper_0100", ret, EOK);
}

/**
 * @tc.name      : isupper_0200
 * @tc.desc      : The parameter c is a lowercase letter,which determines the case of the input character.
 * @tc.level     : Level 2
 */
void isupper_0200(void)
{
    int ret = isupper('a');
    EXPECT_EQ("isupper_0200", ret, EOK);
}

/**
 * @tc.name      : isupper_0300
 * @tc.desc      : The parameter c is a number,which determines the case of the input character.
 * @tc.level     : Level 2
 */
void isupper_0300(void)
{
    int ret = isupper('1');
    EXPECT_EQ("isupper_0300", ret, EOK);
}

/**
 * @tc.name      : isupper_0400
 * @tc.desc      : The parameter c is a special character,which determines the case of the input character.
 * @tc.level     : Level 2
 */
void isupper_0400(void)
{
    int ret = isupper('[');
    EXPECT_EQ("isupper_0400", ret, EOK);
}

/**
 * @tc.name      : isupper_0500
 * @tc.desc      : Determine the number of uppercase letters in the ascii code table.
 * @tc.level     : Level 1
 */
void isupper_0500(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isupper((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isupper_0500", total, COUNT);
}

/**
 * @tc.name      : isupper_l_0100
 * @tc.desc      : Whether the characters in a string is upper characters
 * @tc.level     : Level 0
 */
void isupper_l_0100(void)
{
    const char *str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *p = str;
    while (*p++ && *p != '\0') {
        int ret = isupper_l(*p, NULL);
        EXPECT_EQ("isupper_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : isupper_l_0200
 * @tc.desc      : Whether a character in a string is a non-upper character
 * @tc.level     : Level 2
 */
void isupper_l_0200(void)
{
    const char *str = "23!@abcdefg";
    const char *p = str;
    while (*p++ && *p != '\0') {
        int ret = isupper_l(*p, NULL);
        EXPECT_EQ("isupper_l_0200", ret, CMPFLAG);
    }
}

int main(void)
{
    isupper_0100();
    isupper_0200();
    isupper_0300();
    isupper_0400();
    isupper_0500();
    isupper_l_0100();
    isupper_l_0200();
    return t_status;
}
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

const int COUNT = 62;
const int EOK = 0;

/**
 * @tc.name      : isalnum_0100
 * @tc.desc      : The parameter c is an English letter, and it is judged that the input character is a letter.
 * @tc.level     : Level 0
 */
void isalnum_0100(void)
{
    int ret = isalnum('a');
    EXPECT_NE("isalnum_0100", ret, EOK);
}

/**
 * @tc.name      : isalnum_0200
 * @tc.desc      : The parameter c is an English number, and it is judged that the input character is a number.
 * @tc.level     : Level 0
 */
void isalnum_0200(void)
{
    int ret = isalnum('1');
    EXPECT_NE("isalnum_0200", ret, EOK);
}

/**
 * @tc.name      : isalnum_0300
 * @tc.desc      : The parameter c is an special character,
 *                 and it is judged that the input character is not a letter or a number.
 * @tc.level     : Level 2
 */
void isalnum_0300(void)
{
    int ret = isalnum('*');
    EXPECT_EQ("isalnum_0300", ret, EOK);
}

/**
 * @tc.name      : isalnum_0400
 * @tc.desc      : Determine the number of letters and numbers in the ascii code table.
 * @tc.level     : Level 1
 */
void isalnum_0400(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isalnum((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isalnum_0400", total, COUNT);
}

int main(void)
{
    isalnum_0100();
    isalnum_0200();
    isalnum_0300();
    isalnum_0400();

    return t_status;
}
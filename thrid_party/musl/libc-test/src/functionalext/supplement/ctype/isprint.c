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

#include <ctype.h>
#include "functionalext.h"

const int RET = 0;
const int COUNT = 95;

/**
 * @tc.name      : isprint_0100
 * @tc.desc      : The parameter c is 32, and the input character is judged to be a printable character.
 * @tc.level     : Level 0
 */
void isprint_0100(void)
{
    int ret = isprint(32);
    EXPECT_NE("isprint_0100", ret, RET);
}

/**
 * @tc.name      : isprint_0200
 * @tc.desc      : The parameter c is 63, and the input character is judged to be a printable character.
 * @tc.level     : Level 0
 */
void isprint_0200(void)
{
    int ret = isprint(63);
    EXPECT_NE("isprint_0200", ret, RET);
}

/**
 * @tc.name      : isprint_0300
 * @tc.desc      : The parameter c is 126, and the input character is judged to be a printable character.
 * @tc.level     : Level 2
 */
void isprint_0300(void)
{
    int ret = isprint(126);
    EXPECT_NE("isprint_0300", ret, RET);
}

/**
 * @tc.name      : isprint_0400
 * @tc.desc      : The parameter c is the control character 20, which judges that the input character
 *                 is not a printable character.
 * @tc.level     : Level 2
 */
void isprint_0400(void)
{
    int ret = isprint(20);
    EXPECT_EQ("isprint_0400", ret, RET);
}

/**
 * @tc.name      : isprint_0500
 * @tc.desc      : Determine the number of printable characters in the ascii code table.
 * @tc.level     : Level 1
 */
void isprint_0500(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isprint((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isprint_0500", total, COUNT);
}

int main(void)
{
    isprint_0100();
    isprint_0200();
    isprint_0300();
    isprint_0400();
    isprint_0500();
    return t_status;
}
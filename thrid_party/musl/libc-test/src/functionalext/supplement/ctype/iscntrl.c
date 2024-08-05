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
const int COUNT = 33;

/**
 * @tc.name      : iscntrl_0100
 * @tc.desc      : If the parameter c is 0, it is judged that the input character is a control character.
 * @tc.level     : Level 0
 */
void iscntrl_0100(void)
{
    int ret = iscntrl(0);
    EXPECT_NE("iscntrl_0100", ret, RET);
}

/**
 * @tc.name      : iscntrl_0200
 * @tc.desc      : The parameter c is 31, and the input character is judged to be a control character.
 * @tc.level     : Level 0
 */
void iscntrl_0200(void)
{
    int ret = iscntrl(31);
    EXPECT_NE("iscntrl_0200", ret, RET);
}

/**
 * @tc.name      : iscntrl_0300
 * @tc.desc      : The parameter c is 127, and the input character is judged to be a control character.
 * @tc.level     : Level 2
 */
void iscntrl_0300(void)
{
    int ret = iscntrl(127);
    EXPECT_NE("iscntrl_0300", ret, RET);
}

/**
 * @tc.name      : iscntrl_0400
 * @tc.desc      : The parameter c is a special character, and it is judged that the input
 *                 character is not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_0400(void)
{
    int ret = iscntrl('[');
    EXPECT_EQ("iscntrl_0400", ret, RET);
}

/**
 * @tc.name      : iscntrl_0500
 * @tc.desc      : Determine the number of control characters in the ascii code table.
 * @tc.level     : Level 1
 */
void iscntrl_0500(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = iscntrl((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iscntrl_0500", total, COUNT);
}

int main(void)
{
    iscntrl_0100();
    iscntrl_0200();
    iscntrl_0300();
    iscntrl_0400();
    iscntrl_0500();
    return t_status;
}
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
const int COUNT = 2;

/**
 * @tc.name      : isblank_0100
 * @tc.desc      : The parameter c is a space (' '), and the input character is judged to be a space.
 * @tc.level     : Level 0
 */
void isblank_0100(void)
{
    int ret = isblank(' ');
    EXPECT_NE("isblank_0100", ret, RET);
}

/**
 * @tc.name      : isblank_0200
 * @tc.desc      : The parameter c is a carriage return ('\t'), and the input character is judged to be a space.
 * @tc.level     : Level 0
 */
void isblank_0200(void)
{
    int ret = isblank('\t');
    EXPECT_NE("isblank_0200", ret, RET);
}

/**
 * @tc.name      : isblank_0300
 * @tc.desc      : The parameter c is carriage return ('\r'), which determines that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_0300(void)
{
    int ret = isblank('\r');
    EXPECT_EQ("isblank_0300", ret, RET);
}

/**
 * @tc.name      : isblank_0400
 * @tc.desc      : The parameter c is a newline ('\n'), judging that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_0400(void)
{
    int ret = isblank('\n');
    EXPECT_EQ("isblank_0400", ret, RET);
}

/**
 * @tc.name      : isblank_0500
 * @tc.desc      : The parameter c is the vertical positioning character ('\v'),
 *                 which judges that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_0500(void)
{
    int ret = isblank('\v');
    EXPECT_EQ("isblank_0500", ret, RET);
}

/**
 * @tc.name      : isblank_0600
 * @tc.desc      : The parameter c is page turning ('\f'), which determines that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_0600(void)
{
    int ret = isblank('\f');
    EXPECT_EQ("isblank_0600", ret, RET);
}

/**
 * @tc.name      : isblank_0700
 * @tc.desc      : The parameter c is an English letter, and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_0700(void)
{
    int ret = isblank('a');
    EXPECT_EQ("isblank_0700", ret, RET);
}

/**
 * @tc.name      : isblank_0800
 * @tc.desc      : The parameter c is alphanumeric, and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_0800(void)
{
    int ret = isblank('6');
    EXPECT_EQ("isblank_0800", ret, RET);
}

/**
 * @tc.name      : isblank_0900
 * @tc.desc      : The parameter c is a special character, and it is judged that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_0900(void)
{
    int ret = isblank('#');
    EXPECT_EQ("isblank_0900", ret, RET);
}

/**
 * @tc.name      : isblank_1000
 * @tc.desc      : Determine the number of space characters in the ascii code table.
 * @tc.level     : Level 1
 */
void isblank_1000(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isblank((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isblank_1000", total, COUNT);
}

int main(void)
{
    isblank_0100();
    isblank_0200();
    isblank_0300();
    isblank_0400();
    isblank_0500();
    isblank_0600();
    isblank_0700();
    isblank_0800();
    isblank_0900();
    isblank_1000();
    return t_status;
}
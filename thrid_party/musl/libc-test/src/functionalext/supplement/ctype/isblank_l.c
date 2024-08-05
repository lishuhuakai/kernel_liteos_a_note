/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

const int COUNT = 2;
const int SIZE = 128;

/**
 * @tc.name      : isblank_l_0100
 * @tc.desc      : Verify isblank_l process success. When the parameter c is a space (' '),
 *                 and the input character is judged to be a space.
 * @tc.level     : Level 0
 */
void isblank_l_0100(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l(' ', m_locale);
    EXPECT_NE("isblank_0100", ret, 0);
}

/**
 * @tc.name      : isblank_l_0200
 * @tc.desc      : Verify isblank_l process success. When the parameter c is a carriage return ('\t'),
 *                 and the input character is judged to be a space.
 * @tc.level     : Level0
 */
void isblank_l_0200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l('\t', m_locale);
    EXPECT_NE("isblank_0200", ret, 0);
}

/**
 * @tc.name      : isblank_l_0300
 * @tc.desc      : Verify isblank_l process success. When the parameter c is carriage return ('\r'),
 *           which determines that the input character is not a space.
 * @tc.level     : Level2
 */
void isblank_l_0300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l('\r', m_locale);
    EXPECT_EQ("isblank_0300", ret, 0);
}

/**
 * @tc.name      : isblank_l_0400
 * @tc.desc      : Verify isblank_l process success. When the parameter c is a newline ('\n'),
 *           judging that the input character is not a space.
 * @tc.level     : Level2
 */
void isblank_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l('\n', m_locale);
    EXPECT_EQ("isblank_l_0400", ret, 0);
}

/**
 * @tc.name      : isblank_l_0500
 * @tc.desc      : Verify isblank_l process success. When the parameter c is the vertical positioning character ('\v'),
 *                 which judges that the input character is not a space.
 * @tc.level     : Level 2
 */
void isblank_l_0500(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l('\v', m_locale);
    EXPECT_EQ("isblank_l_0500", ret, 0);
}

/**
 * @tc.name      : isblank_0600
 * @tc.desc      : Verify isblank_l process success. When the parameter c is page turning ('\f'),
 *           which determines that the input character is not a space.
 * @tc.level     : Level2
 */
void isblank_l_0600(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l('\f', m_locale);
    EXPECT_EQ("isblank_l_0600", ret, 0);
}

/**
 * @tc.name      : isblank_l_0700
 * @tc.desc      : Verify isblank_l process success. When the parameter c is an English letter,
 *                 and it is judged that the input character is not a space.
 * @tc.level     : Level2
 */
void isblank_l_0700(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l('a', m_locale);
    EXPECT_EQ("isblank_l_0700", ret, 0);
}

/**
 * @tc.name      : isblank_l_0800
 * @tc.desc      : Verify isblank_l process success. When the parameter c is alphanumeric,
 *                 and it is judged that the input character is not a space.
 * @tc.level     : Level2
 */
void isblank_l_0800(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l('1', m_locale);
    EXPECT_EQ("isblank_0800", ret, 0);
}

/**
 * @tc.name      : isblank_l_0900
 * @tc.desc      : Verify isblank_l process success. When the parameter c is a special character,
 *           and it is judged that the input character is not a space.
 * @tc.level     : Level2
 */
void isblank_l_0900(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isblank_l('#', m_locale);
    EXPECT_EQ("isblank_l_0900", ret, 0);
}

/**
 * @tc.name      : isblank_l_1000
 * @tc.desc      : Verify isblank_l process success. Determine the number of space characters in the ascii code table.
 * @tc.level     : Level1
 */
void isblank_l_1000(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = isblank_l((char)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isblank_l_1000", total, COUNT);
}

int main(void)
{
    isblank_l_0100();
    isblank_l_0200();
    isblank_l_0300();
    isblank_l_0400();
    isblank_l_0500();
    isblank_l_0600();
    isblank_l_0700();
    isblank_l_0800();
    isblank_l_0900();
    isblank_l_1000();
    return t_status;
}
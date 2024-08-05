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

const int COUNT = 95;
const int SIZE = 128;

/**
 * @tc.name      : isprint_l_0100
 * @tc.desc      : Verify isprint_l process success. When the parameter c is 32, and the input character is judged
 *                 to be a printable character.
 * @tc.level     : Level 0
 */
void isprint_l_0100(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isprint_l(32, m_locale);
    EXPECT_NE("isprint_l_0100", ret, 0);
}

/**
 * @tc.name      : isprint_l_0200
 * @tc.desc      : Verify isprint_l process success. Whent the parameter c is 63, and the input character is judged
 *                 to be a printable character.
 * @tc.level     : Level 0
 */
void isprint_l_0200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isprint_l(63, m_locale);
    EXPECT_NE("isprint_l_0200", ret, 0);
}

/**
 * @tc.name      : isprint_l_0300
 * @tc.desc      : Verify isprint_l process success. Whent the parameter c is 126, and the input character is judged
 *                 to be a printable character.
 * @tc.level     : Level 2
 */
void isprint_l_0300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isprint_l(126, m_locale);
    EXPECT_NE("isprint_l_0300", ret, 0);
}

/**
 * @tc.name      : isprint_l_0400
 * @tc.desc      : Verify isprint_l process success. Whent the parameter c is the control character 20, which judges
 *                 that the input character is not a printable character.
 * @tc.level     : Level 2
 */
void isprint_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isprint_l(20, m_locale);
    EXPECT_EQ("isprint_l_0400", ret, 0);
}

/**
 * @tc.name      : isprint_l_0500
 * @tc.desc      : Verify isprint_l process success. Determine the number of printable characters in the ascii code
 *                 table.
 * @tc.level     : Level 1
 */
void isprint_l_0500(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = isprint_l((char)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isprint_l_0500", total, COUNT);
}

int main(void)
{
    isprint_l_0100();
    isprint_l_0200();
    isprint_l_0300();
    isprint_l_0400();
    isprint_l_0500();
    return t_status;
}
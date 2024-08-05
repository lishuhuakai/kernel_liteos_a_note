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

const int COUNT = 33;
const int SIZE = 128;

/**
 * @tc.name      : iscntrl_l_0100
 * @tc.desc      : Verify iscntrl_l process success. When the parameter c is 0,
 *                 it is judged that the input character is a control character.
 * @tc.level     : Level 0
 */
void iscntrl_l_0100(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iscntrl_l(0, m_locale);
    EXPECT_NE("iscntrl_l_0100", ret, 0);
}

/**
 * @tc.name      : iscntrl_l_0200
 * @tc.desc      : Verify iscntrl_l process success. When the parameter c is 31, and the input character
 *                 is judged to be a control character.
 * @tc.level     : Level 0
 */
void iscntrl_l_0200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iscntrl_l(31, m_locale);
    EXPECT_NE("iscntrl_l_0200", ret, 0);
}

/**
 * @tc.name      : iscntrl_l_0300
 * @tc.desc      : Verify iscntrl_l process success. When the parameter c is 127, and the input character
 *                 is judged to be acontrol character.
 * @tc.level     : Level 2
 */
void iscntrl_l_0300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iscntrl_l(127, m_locale);
    EXPECT_NE("iscntrl_l_0300", ret, 0);
}

/**
 * @tc.name      : iscntrl_l_0400
 * @tc.desc      : Verify iscntrl_l process success. When the parameter c is a special character, and it is judged that
 * the input character is not a control character.
 * @tc.level     : Level 2
 */
void iscntrl_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = iscntrl_l('[', m_locale);
    EXPECT_EQ("iscntrl_l_0400", ret, 0);
}

/**
 * @tc.name      : iscntrl_l_0500
 * @tc.desc      : Verify iscntrl_l process success. Determine the number of control characters in ascii code table.
 * @tc.level     : Level 1
 */
void iscntrl_l_0500(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = iscntrl_l((char)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iscntrl_l_0500", total, COUNT);
}

int main(void)
{
    iscntrl_l_0100();
    iscntrl_l_0200();
    iscntrl_l_0300();
    iscntrl_l_0400();
    iscntrl_l_0500();
    return t_status;
}
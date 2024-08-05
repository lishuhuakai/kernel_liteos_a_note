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

const int COUNT = 52;
const int SIZE = 128;

/*
 * @tc.name      : isalpha_l_0100
 * @tc.desc      : Verify isalpha_l process success. When that the entered character is a letter (parameter is 'a').
 * @tc.level     : Level 0
 */
void isalpha_l_0100(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isalpha_l('a', m_locale);
    EXPECT_NE("isalpha_l_0100", ret, 0);
}

/*
 * @tc.name      : isalpha_l_0200
 * @tc.desc      : Verify isalpha_l process success. When the entered character is not a letter (parameter is '1').
 * @tc.level     : Level 2
 */
void isalpha_l_0200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isalpha_l('1', m_locale);
    EXPECT_EQ("isalpha_l_0200", ret, 0);
}

/*
 * @tc.name      : isalpha_l_0300
 * @tc.desc      : Verify isalpha_l process success. When the entered character is not a letter (parameter is '【').
 * @tc.level     : Level 2
 */
void isalpha_l_0300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isalpha_l('[', m_locale);
    EXPECT_EQ("isalpha_l_0300", ret, 0);
}

/*
 * @tc.name      : isalpha_l_0400
 * @tc.desc      : Verify isalpha_l process success. Verify the number of letters in the ascii code table.
 * @tc.level     : Level 1
 */
void isalpha_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = isalpha_l((char)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isalpha_l_0400", total, COUNT);
}

int main(void)
{
    isalpha_l_0100();
    isalpha_l_0200();
    isalpha_l_0300();
    isalpha_l_0400();
    return t_status;
}
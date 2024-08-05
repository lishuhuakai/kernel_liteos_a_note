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

const int SIZE = 128;
const int RETSIZE = 6;

/*
 * @tc.name      : isspace_l_0100
 * @tc.desc      : Verify isprint_l process success. When the input character is a space (the parameter is a space)
 * @tc.level     : Level 0
 */
void isspace_l_0100(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isspace_l(' ', m_locale);
    EXPECT_NE("isspace_l_0100", ret, 0);
}

/*
 * @tc.name      : isspace_l_0200
 * @tc.desc      : Verify isprint_l process success. When the input character is a space (parameter is \r)
 * @tc.level     : Level 1
 */
void isspace_l_0200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isspace_l('\r', m_locale);
    EXPECT_NE("isspace_l_0200", ret, 0);
}

/*
 * @tc.name      : isspace_l_0300
 * @tc.desc      : Verify isprint_l process success. When the input character is a space (parameter is \n)
 * @tc.level     : Level 1
 */
void isspace_l_0300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isspace_l('\n', m_locale);
    EXPECT_NE("isspace_l_0300", ret, 0);
}

/*
 * @tc.name      : isspace_l_0400
 * @tc.desc      : Verify isprint_l process success. When the input character is a space (parameter is \v)
 * @tc.level     : Level 1
 */
void isspace_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isspace_l('\v', m_locale);
    EXPECT_NE("isspace_l_0400", ret, 0);
}

/*
 * @tc.name      : isspace_l_0500
 * @tc.desc      : Verify isprint_l process success. When the input character is a space (parameter is \f)
 * @tc.level     : Level 1
 */
void isspace_l_0500(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isspace_l('\f', m_locale);
    EXPECT_NE("isspace_l_0500", ret, 0);
}

/*
 * @tc.name      : isspace_l_0600
 * @tc.desc      : Verify isprint_l process success. When the input character is not a space (parameter is 'a')
 * @tc.level     : Level 2
 */
void isspace_l_0600(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isspace_l('a', m_locale);
    EXPECT_EQ("isspace_l_0600", ret, 0);
}

/*
 * @tc.name      : isspace_l_0700
 * @tc.desc      : Verify isprint_l process success. When the input character is not a space (parameter is '6')
 * @tc.level     : Level 2
 */
void isspace_l_0700(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isspace_l('6', m_locale);
    EXPECT_EQ("isspace_l_0700", ret, 0);
}

/*
 * @tc.name      : isspace_l_0800
 * @tc.desc      : Verify isprint_l process success. When the input character is not a space (parameter is '#')
 * @tc.level     : Level 2
 */
void isspace_l_0800(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isspace_l('#', m_locale);
    EXPECT_EQ("isspace_l_0800", ret, 0);
}

/*
 * @tc.name      : isspace_l_0900
 * @tc.desc      : Verify isprint_l process success. Verify the number of characters in the ascii code table.
 * @tc.level     : Level 1
 */
void isspace_l_0900()
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = isspace_l((char)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isspace_l_0900", total, RETSIZE);
}

int main(void)
{
    isspace_l_0100();
    isspace_l_0200();
    isspace_l_0300();
    isspace_l_0400();
    isspace_l_0500();
    isspace_l_0600();
    isspace_l_0700();
    isspace_l_0800();
    isspace_l_0900();
    return t_status;
}
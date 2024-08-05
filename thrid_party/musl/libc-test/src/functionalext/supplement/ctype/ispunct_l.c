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
const int RESULT_SIZE = 32;

/*
 * @tc.name      : ispunct_l_0100
 * @tc.desc      : Verify ispunct_l process success. When the input character is a punctuation mark or a special symbol.
 * @tc.level     : Level 0
 */
void ispunct_l_0100(void)
{
    int var1 = 44;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = ispunct_l(var1, m_locale);
    EXPECT_NE("ispunct_l_0100", ret, 0);
}

/*
 * @tc.name      : ispunct_l_0200
 * @tc.desc      : Verify ispunct_l process success. When the input character is a punctuation mark or a special symbol.
 * @tc.level     : Level 0
 */
void ispunct_l_0200(void)
{
    int var1 = 64;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = ispunct_l(var1, m_locale);
    EXPECT_NE("ispunct_l_0200", ret, 0);
}

/*
 * @tc.name      : ispunct_l_0300
 * @tc.desc      : Verify ispunct_l process success. When the input character is a punctuation mark or a special symbol.
 * @tc.level     : Level 0
 */
void ispunct_l_0300(void)
{
    int var1 = 94;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = ispunct_l(var1, m_locale);
    EXPECT_NE("ispunct_l_0300", ret, 0);
}

/*
 * @tc.name      : ispunct_l_0400
 * @tc.desc      : Verify ispunct_l process success. When input character is not a punctuation mark or a special symbol.
 * @tc.level     : Level 2
 */
void ispunct_l_0400(void)
{
    int var1 = 65;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = ispunct_l(var1, m_locale);
    EXPECT_EQ("ispunct_l_0400", ret, 0);
}

/*
 * @tc.name      : ispunct_l_0500
 * @tc.desc      : Verify ispunct_l process success. When the number of punctuation marks or special symbols
 *                 in the ascii code table.
 * @tc.level     : Level 1
 */
void ispunct_l_0500(void)
{
    int cout = 0;
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    for (int i = 0; i < SIZE; i++) {
        if (ispunct_l(i, m_locale)) {
            cout++;
        }
    }
    EXPECT_EQ("ispunct_l_0500", cout, RESULT_SIZE);
}

int main(void)
{
    ispunct_l_0100();
    ispunct_l_0200();
    ispunct_l_0300();
    ispunct_l_0400();
    ispunct_l_0500();
    return t_status;
}
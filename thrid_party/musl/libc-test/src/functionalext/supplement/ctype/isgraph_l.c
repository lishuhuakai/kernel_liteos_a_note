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
#include <wchar.h>
#include "functionalext.h"

const int COUNT = 94;
const int SIZE = 128;

/*
 * @tc.name      : isgraph_l_0100
 * @tc.desc      : Verify isgraph_l process success. When that the entered character is a letter (parameter is 'a'
 *                 has a graphical representation).
 * @tc.level     : Level 0
 */
void isgraph_l_0100(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isgraph_l('a', m_locale);
    EXPECT_NE("isgraph_l_0100", ret, 0);
}

/*
 * @tc.name      : isgraph_l_0200
 * @tc.desc      : Verify isgraph_l process success. When that the entered character is not a letter (parameter is '!'
 *                 has a graphical representation).
 * @tc.level     : Level 0
 */
void isgraph_l_0200(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isgraph_l('!', m_locale);
    EXPECT_NE("isgraph_l_0200", ret, 0);
}

/*
 * @tc.name      : isgraph_l_0300
 * @tc.desc      : Verify isgraph_l process success. When that the entered character is not has a graphical representation.
 * @tc.level     : Level 2
 */
void isgraph_l_0300(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int ret = isgraph_l(' ', m_locale);
    EXPECT_EQ("isgraph_l_0300", ret, 0);
}

/**
 * @tc.name      : isgraph_l_0400
 * @tc.desc      : Verify isgraph_l process success. Determine the number of control characters in the ascii code table.
 * @tc.level     : Level 1
 */
void isgraph_l_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = isgraph_l((char)i, m_locale);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isgraph_l_0400", total, COUNT);
}

int main(void)
{
    isgraph_l_0100();
    isgraph_l_0200();
    isgraph_l_0300();
    isgraph_l_0400();
    return t_status;
}
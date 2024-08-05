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

#include <locale.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "functionalext.h"

const int COUNT = 94;
const int SIZE = 128;

/*
 * @tc.name      : iswgraph_0100
 * @tc.desc      : Verify iswgraph process success. When that the entered character is a letter (parameter is 'a'
 *                 has a graphical representation).
 * @tc.level     : Level 0
 */
void iswgraph_0100(void)
{
    wchar_t c = 'a';
    int ret = iswgraph(c);
    EXPECT_NE("iswgraph_0100", ret, 0);
}

/*
 * @tc.name      : iswgraph_0200
 * @tc.desc      : Verify iswgraph process success. When that the entered character is not a letter (parameter is '!'
 *                 has a graphical representation).
 * @tc.level     : Level 0
 */
void iswgraph_0200(void)
{
    wchar_t c = '!';
    int ret = iswgraph(c);
    EXPECT_NE("iswgraph_0200", ret, 0);
}

/*
 * @tc.name      : iswgraph_0300
 * @tc.desc      : Verify iswgraph process success. When that the entered character is not has a graphical representation.
 * @tc.level     : Level 2
 */
void iswgraph_0300(void)
{
    wchar_t c = ' ';
    int ret = iswgraph(c);
    EXPECT_EQ("iswgraph_0300", ret, 0);
}

/**
 * @tc.name      : iswgraph_0400
 * @tc.desc      : Verify iswgraph process success. Determine the number of control characters in the ascii code table.
 * @tc.level     : Level 1
 */
void iswgraph_0400(void)
{
    locale_t m_locale = newlocale(LC_ALL_MASK, "en_US.UTF-8", NULL);
    int total = 0;
    for (int i = 0; i < SIZE; i++) {
        int ret = iswgraph((wchar_t)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("iswgraph_0400", total, COUNT);
}

int main(void)
{
    iswgraph_0100();
    iswgraph_0200();
    iswgraph_0300();
    iswgraph_0400();
    return t_status;
}
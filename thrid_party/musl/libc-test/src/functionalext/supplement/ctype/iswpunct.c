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
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

#include "functionalext.h"
#include "test.h"

/**
 * @tc.name      : iswpunct_0100
 * @tc.desc      : Use the iswpunct method to determine whether the incoming wide character is a punctuation mark
 * @tc.level     : Level 0
 */
void iswpunct_0100(void)
{
    wchar_t ch = L',';
    int result = iswpunct(ch);
    if (result == 0) {
        t_error("%s iswpunct get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswpunct_0200
 * @tc.desc      : Test iswpunct method for wide characters passed in non-punctuation marks
 * @tc.level     : Level 1
 */
void iswpunct_0200(void)
{
    wchar_t ch = L'A';
    int result = iswpunct(ch);
    if (result != 0) {
        t_error("%s iswpunct get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswpunct_l_0100
 * @tc.desc      : Whether the characters in a wide string are punctuation marks
 * @tc.level     : Level 0
 */
void iswpunct_l_0100(void)
{
    const wchar_t *str = L"!！，。？?";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswpunct_l(*p, NULL);
        EXPECT_EQ("iswpunct_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswpunct_l_0200
 * @tc.desc      : Whether the characters in a wide string are non-punctuation marks
 * @tc.level     : Level 2
 */
void iswpunct_l_0200(void)
{
    const wchar_t *str = L"1AaÂÆ";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswpunct_l(*p, NULL);
        EXPECT_EQ("iswpunct_l_0200", ret, CMPFLAG);
    }
}

int main(int argc, char *argv[])
{
    iswpunct_0100();
    iswpunct_0200();
    iswpunct_l_0100();
    iswpunct_l_0200();
    return t_status;
}
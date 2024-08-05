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
 * @tc.name      : iswspace_0100
 * @tc.desc      : Use the iswspace method to determine whether the incoming wide character is a blank symbol
 * @tc.level     : Level 0
 */
void iswspace_0100(void)
{
    wchar_t ch = L' ';
    int result = iswspace(ch);
    if (result == 0) {
        t_error("%s iswspace get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswspace_0200
 * @tc.desc      : Test iswpunct method for wide characters passing in non-whitespace characters
 * @tc.level     : Level 1
 */
void iswspace_0200(void)
{
    wchar_t ch = L'A';
    int result = iswspace(ch);
    if (result != 0) {
        t_error("%s iswspace get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswspace_l_0100
 * @tc.desc      : Whether the characters in a wide string is whitespace character
 * @tc.level     : Level 0
 */
void iswspace_l_0100(void)
{
    const wchar_t *str = L" \t\n\r";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswspace_l(*p, NULL);
        EXPECT_EQ("iswspace_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswspace_l_0200
 * @tc.desc      : Whether the characters in a wide string are non-whitespace character
 * @tc.level     : Level 2
 */
void iswspace_l_0200(void)
{
    const wchar_t *str = L"1AaÂÆ";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswspace_l(*p, NULL);
        EXPECT_EQ("iswspace_l_0200", ret, CMPFLAG);
    }
}

int main(int argc, char *argv[])
{
    iswspace_0100();
    iswspace_0200();
    iswspace_l_0100();
    iswspace_l_0200();
    return t_status;
}
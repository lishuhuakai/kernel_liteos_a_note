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
 * @tc.name      : iswupper_0100
 * @tc.desc      : Determine whether the incoming wide character is an uppercase letter by the iswupper method
 * @tc.level     : Level 0
 */
void iswupper_0100(void)
{
    wchar_t ch = L'A';
    int result = iswupper(ch);
    if (result == 0) {
        t_error("%s iswupper get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswupper_0200
 * @tc.desc      : Test the return value of iswupper when lowercase letters are passed in
 * @tc.level     : Level 1
 */
void iswupper_0200(void)
{
    wchar_t ch = L'a';
    int result = iswupper(ch);
    if (result != 0) {
        t_error("%s iswupper get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswupper_0300
 * @tc.desc      : Test iswupper return value when passing in a number
 * @tc.level     : Level 1
 */
void iswupper_0300(void)
{
    wchar_t ch = L'1';
    int result = iswupper(ch);
    if (result != 0) {
        t_error("%s iswupper get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswupper_l_0100
 * @tc.desc      : Whether the characters in a wide string is upper character
 * @tc.level     : Level 0
 */
void iswupper_l_0100(void)
{
    const wchar_t *str = L"ABCDEFG";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswupper_l(*p, NULL);
        EXPECT_EQ("iswupper_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswupper_l_0200
 * @tc.desc      : Whether the characters in a wide string are non-upper character
 * @tc.level     : Level 2
 */
void iswupper_l_0200(void)
{
    const wchar_t *str = L"abcefg1";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswupper_l(*p, NULL);
        EXPECT_EQ("iswupper_l_0200", ret, CMPFLAG);
    }
}

int main(int argc, char *argv[])
{
    iswupper_0100();
    iswupper_0200();
    iswupper_0300();
    iswupper_l_0100();
    iswupper_l_0200();
    return t_status;
}
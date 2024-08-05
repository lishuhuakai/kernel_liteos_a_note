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

#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>

#include "functionalext.h"
#include "test.h"

/**
 * @tc.name      : iswlower_0100
 * @tc.desc      : Use the iswlower method to determine whether the incoming wide character is a lowercase letter
 * @tc.level     : Level 0
 */
void iswlower_0100(void)
{
    wchar_t ch = L'a';
    int result = iswlower(ch);
    if (result == 0) {
        t_error("%s iswlower get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswlower_0200
 * @tc.desc      : Test the return value of iswlower when uppercase letters are passed in
 * @tc.level     : Level 1
 */
void iswlower_0200(void)
{
    wchar_t ch = L'A';
    int result = iswlower(ch);
    if (result != 0) {
        t_error("%s iswlower get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswlower_0300
 * @tc.desc      : Test the return value of iswlower when passing in a number
 * @tc.level     : Level 1
 */
void iswlower_0300(void)
{
    wchar_t ch = L'1';
    int result = iswlower(ch);
    if (result != 0) {
        t_error("%s iswlower get result is %d error", __func__, result);
    }
}

/**
 * @tc.name      : iswlower_l_0100
 * @tc.desc      : Whether the characters in a wide string is lower character
 * @tc.level     : Level 0
 */
void iswlower_l_0100(void)
{
    const wchar_t *str = L"abcdefghijklmnopqrstuvwxyz";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswlower_l(*p, NULL);
        EXPECT_EQ("iswlower_l_0100", ret, ONREXPECT);
    }
}

/**
 * @tc.name      : iswlower_l_0200
 * @tc.desc      : Whether the characters in a wide string are non-lower character
 * @tc.level     : Level 2
 */
void iswlower_l_0200(void)
{
    const wchar_t *str = L"12ABC!@#";
    const wchar_t *p = str;
    while (*p++ && *p != '\0') {
        int ret = iswlower_l(*p, NULL);
        EXPECT_EQ("iswlower_l_0200", ret, CMPFLAG);
    }
}

int main(int argc, char *argv[])
{
    iswlower_0100();
    iswlower_0200();
    iswlower_0300();
    iswlower_l_0100();
    iswlower_l_0200();
    return t_status;
}
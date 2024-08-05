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

#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : wcspbrk_0100
 * @tc.desc      : Test wcspbrk to get the first occurrence of the target character in the source string
 * @tc.level     : Level 0
 */
void wcspbrk_0100(void)
{
    const wchar_t *s = L"hello, world!";
    wchar_t *result = wcspbrk(s, L"-");
    if (result) {
        t_error("%s wcspbrk get are not null\n", __func__);
    }
}

/**
 * @tc.name      : wcspbrk_0200
 * @tc.desc      : Test the result of wcspbrk when the target character appears at the first position of the source
 *                 character
 * @tc.level     : Level 1
 */
void wcspbrk_0200(void)
{
    const wchar_t *s = L"hello, world!";
    wchar_t *result = wcspbrk(s, L"abch");
    if (wcscmp(result, s) != 0) {
        t_error("%s wcspbrk get result is %lc are not %lc\n", __func__, result, s);
    }
}

/**
 * @tc.name      : wcspbrk_0300
 * @tc.desc      : Test the result of wcspbrk when the target character appears in the middle of the source character
 * @tc.level     : Level 1
 */
void wcspbrk_0300(void)
{
    const wchar_t *s = L"hello, world!";
    wchar_t *result = wcspbrk(s, L"l");
    if (wcscmp(result, s + 2) != 0) {
        t_error("%s wcspbrk get result is %lc are not %lc\n", __func__, result, s + 2);
    }
}

/**
 * @tc.name      : wcspbrk_0400
 * @tc.desc      : Test the result of wcspbrk when the target character is punctuation
 * @tc.level     : Level 1
 */
void wcspbrk_0400(void)
{
    const wchar_t *s = L"hello, world!";
    wchar_t *result = wcspbrk(s, L",. !");
    if (wcscmp(result, s + 5) != 0) {
        t_error("%s wcspbrk get result is %lc are not %lc\n", __func__, result, s + 5);
    }
}

int main(int argc, char *argv[])
{
    wcspbrk_0100();
    wcspbrk_0200();
    wcspbrk_0300();
    wcspbrk_0400();
    return t_status;
}
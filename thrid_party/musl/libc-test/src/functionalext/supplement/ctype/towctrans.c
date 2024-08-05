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
#include "test.h"

/**
 * @tc.name      : towctrans_0100
 * @tc.desc      : Convert character of current type to lowercases
 * @tc.level     : Level 0
 */
void towctrans_0100(void)
{
    wchar_t str[] = L"ABCD";
    wctrans_t trans = wctrans("tolower");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"abcd")) {
        t_error("%s towctrans lower failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0200
 * @tc.desc      : Convert character of current type to uppercase
 * @tc.level     : Level 1
 */
void towctrans_0200(void)
{
    wchar_t str[] = L"abcd";
    wctrans_t trans = wctrans("toupper");
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], trans);
    }
    if (wcscmp(str, L"ABCD")) {
        t_error("%s towctrans upper failed", __func__);
    }
}

/**
 * @tc.name      : towctrans_0300
 * @tc.desc      : Convert character of current type to uppercase
 * @tc.level     : Level 1
 */
void towctrans_0300(void)
{
    wchar_t str[] = L"Abcd";
    for (int i = 0; i < wcslen(str); i++) {
        str[i] = towctrans(str[i], 0);
    }
    if (wcscmp(str, L"Abcd")) {
        t_error("%s towctrans failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    towctrans_0100();
    towctrans_0200();
    towctrans_0300();
    return t_status;
}
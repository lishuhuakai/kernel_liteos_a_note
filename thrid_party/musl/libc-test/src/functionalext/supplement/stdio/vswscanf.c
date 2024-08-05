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
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <locale.h>
#include <wchar.h>
#include "test.h"

int get_str_value(const wchar_t *buffer, const wchar_t *format, ...)
{
    int result = 0;
    va_list args;
    va_start(args, format);
    vswscanf(buffer, format, args);
    va_end(args);
    return result;
}

/**
 * @tc.name      : vswscanf_0100
 * @tc.desc      : Test the vswscanf function to read data from the wide character buffer and store it in the
 *                 corresponding variable
 * @tc.level     : Level 0
 */
void vswscanf_0100(void)
{
    wchar_t symbol[] = L"\u0fd7\u00c6\u20b9\u2127\u17d8";
    wchar_t str[20];
    int result = get_str_value(symbol, L"%ls", str);
    if (result != 0) {
        t_error("%s vswscanf get result is %d are not want 0\n", __func__, result);
    }
    if (wcscmp(symbol, str) != 0) {
        t_error("%s vswscanf error %ls are not eq %ls\n", __func__, symbol, str);
    }
}

/**
 * @tc.name      : vswscanf_0200
 * @tc.desc      : Test the result of wswscanf when fetching multiple variables from a wide character buffer
 * @tc.level     : Level 1
 */
void vswscanf_0200(void)
{
    wchar_t symbol[] = L"ABC DEF";
    wchar_t str[20];
    wchar_t strs[20];
    int result = get_str_value(symbol, L"%ls %ls", str, strs);
    if (result != 0) {
        t_error("%s vswscanf get result is %d are not want 0\n", __func__, result);
    }
    if (wcscmp(str, L"ABC") != 0) {
        t_error("%s vswscanf error %ls are not eq ABC\n", __func__, str);
    }
    if (wcscmp(strs, L"DEF") != 0) {
        t_error("%s vswscanf error %ls are not eq DEF\n", __func__, strs);
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_US.UTF-8");
    vswscanf_0100();
    vswscanf_0200();
    return t_status;
}
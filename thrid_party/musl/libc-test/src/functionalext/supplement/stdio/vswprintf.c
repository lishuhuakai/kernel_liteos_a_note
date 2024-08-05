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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "test.h"

#define MAX (20)

/**
 * @tc.name      : vswprintf_0100
 * @tc.desc      : Write formatted data %d to string from variable argument list
 * @tc.level     : Level 0
 */
void vswprintf_0100(wchar_t *format, ...)
{
    wchar_t buffer[MAX];
    va_list aptr;
    va_start(aptr, format);
    int result = vswprintf(buffer, MAX, format, aptr);
    va_end(aptr);
    if (result < 0) {
        t_error("%s vswprintf get result is less than 0", __func__);
    }
    if (wcscmp(buffer, L"1")) {
        t_error("%s wrong string written to buf", __func__);
    }
}

/**
 * @tc.name      : vswprintf_0200
 * @tc.desc      : Write formatted data %f to string from variable argument list
 * @tc.level     : Level 1
 */
void vswprintf_0200(wchar_t *format, ...)
{
    wchar_t buffer[MAX];
    va_list aptr;
    va_start(aptr, format);
    int result = vswprintf(buffer, MAX, format, aptr);
    va_end(aptr);
    if (result < 0) {
        t_error("%s vswprintf get result is less than 0", __func__);
    }
    if (wcsncmp(buffer, L"3.0", 3)) {
        t_error("%s wrong string written to buf", __func__);
    }
}

/**
 * @tc.name      : vswprintf_0300
 * @tc.desc      : Write formatted data %s to string from variable argument list
 * @tc.level     : Level 1
 */
void vswprintf_0300(wchar_t *format, ...)
{
    wchar_t buffer[MAX];
    va_list aptr;
    va_start(aptr, format);
    int result = vswprintf(buffer, MAX, format, aptr);
    va_end(aptr);
    if (result < 0) {
        t_error("%s vsprintf get result is less than 0", __func__);
    }
    if (wcscmp(buffer, L"vswprintf test")) {
        t_error("%s wrong string written to buf", __func__);
    }
}

int main(int argc, char *argv[])
{
    int i = 1;
    float f = 3.0f;
    wchar_t str[] = L"vswprintf test";
    vswprintf_0100(L"%d", i);
    vswprintf_0200(L"%f", f);
    vswprintf_0300(L"%ls", str);
    return t_status;
}
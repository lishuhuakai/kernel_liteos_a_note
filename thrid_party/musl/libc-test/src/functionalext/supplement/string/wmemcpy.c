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
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wmemcpy_0100
 * @tc.desc      : Copies specified number of characters from one string to another
 * @tc.level     : Level 0
 */
void wmemcpy_0100(void)
{
    wchar_t dest[30];
    wchar_t src[] = L"This is a c test for wmemcpy";
    int n = 13;
    wmemcpy(dest, src, n);
    if (wcsncmp(dest, src, n)) {
        t_error("%s The string of dest is not equal to src\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0200
 * @tc.desc      : Test wmemcpy when 'n' is equal to the number characters in src string
 * @tc.level     : Level 0
 */
void wmemcpy_0200(void)
{
    wchar_t dest[3];
    wchar_t src[] = L"aaa";
    int n = 3;
    wmemcpy(dest, src, n);
    if (wcsncmp(dest, src, n)) {
        t_error("%s The string of dest is not equal to src\n", __func__);
    }
}

/**
 * @tc.name      : wmemcpy_0300
 * @tc.desc      : Test wmemcpy when 'n' is equal to 0
 * @tc.level     : Level 0
 */
void wmemcpy_0300(void)
{
    wchar_t dest[3];
    wchar_t src[] = L"aaa";
    int n = 0;
    wmemcpy(dest, src, n);
    if (!wcscmp(dest, src)) {
        t_error("%s The string of dest is equal to src\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wmemcpy_0100();
    wmemcpy_0200();
    wmemcpy_0300();
    return t_status;
}
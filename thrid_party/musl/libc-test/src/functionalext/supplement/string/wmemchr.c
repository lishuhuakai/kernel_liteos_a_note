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
 * @tc.name      : wmemchr_0100
 * @tc.desc      : Find the first occurrence of a wide character in the target array
 * @tc.level     : Level 0
 */
void wmemchr_0100(void)
{
    wchar_t str[] = L"ABCDEFG";
    wchar_t target = L'C';
    size_t sz = sizeof(str) / sizeof(*str);
    wchar_t *result = wmemchr(str, target, sz);
    if (!result) {
        t_error("%s not found target ch\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0200
 * @tc.desc      : Test the return value of the function when there are no wide characters in the destination array
 * @tc.level     : Level 1
 */
void wmemchr_0200(void)
{
    wchar_t str[] = L"ABCDEFG";
    wchar_t target = L'H';
    size_t sz = sizeof(str) / sizeof(*str);
    wchar_t *result = wmemchr(str, target, sz);
    if (result) {
        t_error("%s found target ch\n", __func__);
    }
}

/**
 * @tc.name      : wmemchr_0300
 * @tc.desc      : Test the return value of the function when n=0
 * @tc.level     : Level 1
 */
void wmemchr_0300(void)
{
    wchar_t str[] = L"ABCDEFG";
    wchar_t target = L'C';
    size_t sz = 0;
    wchar_t *result = wmemchr(str, target, sz);
    if (result) {
        t_error("%s found target ch", __func__);
    }
}

int main(int argc, char *argv[])
{
    wmemchr_0100();
    wmemchr_0200();
    wmemchr_0300();
    return t_status;
}
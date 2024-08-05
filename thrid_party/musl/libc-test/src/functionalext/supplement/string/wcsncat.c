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
 * @tc.name      : wcsncat_0100
 * @tc.desc      : Test the wcsncat method to pass the source wide string into the target wide string according to the
 *                 number passed in
 * @tc.level     : Level 0
 */
void wcsncat_0100(void)
{
    wchar_t dst[32] = {0};
    wchar_t *result = wcsncat(dst, L"hello, world!", 5);
    if (wcscmp(dst, result) != 0) {
        t_error("%s wcsncat get result is %s are not %s\n", __func__, dst, result);
    }
    if (wcscmp(dst, L"hello") != 0) {
        t_error("%s wcsncat get result is %s are not 'hello'\n", __func__, dst);
    }
}

/**
 * @tc.name      : wcsncat_0200
 * @tc.desc      : Test the result of the function wcsncat when the number of incoming copies is 0
 * @tc.level     : Level 1
 */
void wcsncat_0200(void)
{
    wchar_t dst[32] = {0};
    wchar_t *result = wcsncat(dst, L"hello, world!", 0);
    if (wcscmp(dst, result) != 0) {
        t_error("%s wcsncat get result is %s are not %s\n", __func__, dst, result);
    }
    if (wcscmp(dst, L"") != 0) {
        t_error("%s wcsncat get result is %s are not 'hello'\n", __func__, dst);
    }
}

/**
 * @tc.name      : wcsncat_0300
 * @tc.desc      : Test the result of the wcsncat function when the number of incoming copies is greater than the 
 *                 source string
 * @tc.level     : Level 1
 */
void wcsncat_0300(void)
{
    wchar_t dst[32] = {0};
    wchar_t *result = wcsncat(dst, L"hello, world!", 16);
    if (wcscmp(dst, result) != 0) {
        t_error("%s wcsncat get result is %s are not %s\n", __func__, dst, result);
    }
    if (wcscmp(dst, L"hello, world!") != 0) {
        t_error("%s wcsncat get result is %s are not 'hello, world!'\n", __func__, dst);
    }
}

int main(int argc, char *argv[])
{
    wcsncat_0100();
    wcsncat_0200();
    wcsncat_0300();
    return t_status;
}
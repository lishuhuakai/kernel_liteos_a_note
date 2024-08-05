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
 * @tc.name      : wcpncpy_0100
 * @tc.desc      : Test the wcpncpy method to copy a wide string in bits
 * @tc.level     : Level 0
 */
void wcpncpy_0100(void)
{
    wchar_t src[] = L"Source string";
    wchar_t dst[18];
    size_t src_len = 13;
    wchar_t *result = wcpncpy(dst, src, src_len + 1);
    if (wcscmp(result, dst + src_len) != 0) {
        t_error("%s wcpncpy get result is %ls are not want %ls\n", __func__, result, dst + src_len);
    }
    if (wcscmp(src, dst) != 0) {
        t_error("%s wcpncpy get dst is %ls are not want %ls\n", __func__, dst, src);
    }
}

/**
 * @tc.name      : wcpncpy_0200
 * @tc.desc      : Test the result of the wcpncpy method when the incoming length is less than the copied wide string
 * @tc.level     : Level 1
 */
void wcpncpy_0200(void)
{
    wchar_t src[] = L"Source string";
    wchar_t dst[18];
    size_t src_len = 13;
    wchar_t *result = wcpncpy(dst, src, 2);
    if (wcscmp(result, dst + 2) != 0) {
        t_error("%s wcpncpy get result is %ls are not want %ls\n", __func__, result, dst + 2);
    }
    if (wcscmp(dst, L"So") != 0) {
        t_error("%s wcpncpy get dst is %ls are not want %ls\n", __func__, dst, src);
    }
}

/**
 * @tc.name      : wcpncpy_0300
 * @tc.desc      : Test the result of the wcpncpy method when the incoming length is greater than the copied wide string
 * @tc.level     : Level 1
 */
void wcpncpy_0300(void)
{
    wchar_t src[] = L"Source string";
    wchar_t dst[18];
    size_t src_len = 13;
    wchar_t *result = wcpncpy(dst, src, 17);
    if (wcscmp(result, dst + src_len) != 0) {
        t_error("%s wcpncpy get result is %ls are not want %ls\n", __func__, result, dst + src_len);
    }
    if (wcscmp(src, dst) != 0) {
        t_error("%s wcpncpy get dst is %ls are not want %ls\n", __func__, dst, src);
    }
}

int main(int argc, char *argv[])
{
    wcpncpy_0100();
    wcpncpy_0200();
    wcpncpy_0300();
    return t_status;
}
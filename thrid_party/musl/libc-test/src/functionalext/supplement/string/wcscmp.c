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
 * @tc.name      : wcscmp_0100
 * @tc.desc      : Compares two equal wide strings and returns a value equal to 0
 * @tc.level     : Level 0
 */
void wcscmp_0100(void)
{
    wchar_t str1[] = L"test wcscmp";
    wchar_t str2[] = L"test wcscmp";
    if (wcscmp(str1, str2)) {
        t_error("%s error, the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcscmp_0200
 * @tc.desc      : Compares two equal wide strings and returns a value less than zero
 * @tc.level     : Level 0
 */
void wcscmp_0200(void)
{
    wchar_t str1[] = L"ABCD";
    wchar_t str2[] = L"ABCE";
    if (wcscmp(str1, str2) >= 0) {
        t_error("%s error, expected result value less than zero", __func__);
    }
}

/**
 * @tc.name      : wcscmp_0300
 * @tc.desc      : Compares two equal wide strings and returns a value greater than zero
 * @tc.level     : Level 0
 */
void wcscmp_0300(void)
{
    wchar_t str1[] = L"ABCE";
    wchar_t str2[] = L"ABCD";
    if (wcscmp(str1, str2) <= 0) {
        t_error("%s error, expected result value greater than zero", __func__);
    }
}

/**
 * @tc.name      : wcscmp_0400
 * @tc.desc      : The first characters of the compared wide strings are different
 * @tc.level     : Level 1
 */
void wcscmp_0400(void)
{
    wchar_t str1[] = L"ABCD";
    wchar_t str2[] = L"BCDE";
    if (wcscmp(str1, str2) >= 0) {
        t_error("%s error, expected result value less than zero", __func__);
    }
    if (wcscmp(str2, str1) <= 0) {
        t_error("%s error, expected result value greater than zero", __func__);
    }
}

/**
 * @tc.name      : wcscmp_0500
 * @tc.desc      : One or both of the wide strings compared are equal to NULL
 * @tc.level     : Level 1
 */
void wcscmp_0500(void)
{
    wchar_t str1[] = L"";
    wchar_t str2[] = L"";
    wchar_t str3[] = L"A";
    if (wcscmp(str1, str3) >= 0) {
        t_error("%s error, expected result value less than zero", __func__);
    }
    if (wcscmp(str3, str1) <= 0) {
        t_error("%s error, expected result value greater than zero", __func__);
    }
    if (wcscmp(str1, str2)) {
        t_error("%s error, the result of comparing two strings should be equal", __func__);
    }
}

int main(int argc, char *argv[])
{
    wcscmp_0100();
    wcscmp_0200();
    wcscmp_0300();
    wcscmp_0400();
    wcscmp_0500();
    return t_status;
}
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
#include <locale.h>
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wcrtomb_0100
 * @tc.desc      : call wcrtomb to convert wide characters to narrow characters
 * @tc.level     : Level 0
 */
void wcrtomb_0100(void)
{
    wchar_t wcs = L'A';
    char s[20];
    mbstate_t pr;
    memset(&pr, 0, sizeof pr);
    int result = wcrtomb(s, wcs, &pr);
    if (result != 1) {
        t_error("%s wcrtomb get result is %d are not 1\n", __func__, result);
    }
    if (strcmp(s, "A") != 0) {
        t_error("%s wcrtomb error get s is '%s' are not 'A'\n", __func__, s);
    }
}

/**
 * @tc.name      : wcrtomb_0200
 * @tc.desc      : Wide character length exceeds 0x80
 * @tc.level     : Level 1
 */
void wcrtomb_0200(void)
{
    wchar_t wcs = L'\u00df';
    int want = 2;
    char s[20];
    mbstate_t pr;
    memset(&pr, 0, sizeof pr);
    int result = wcrtomb(s, wcs, &pr);
    if (result != want) {
        t_error("%s wcrtomb get result is %d are not 2\n", __func__, result);
    }
    if (strcmp(s, "ß") != 0) {
        t_error("%s wcrtomb error get s is '%s' are not 'ß'\n", __func__, s);
    }
}

/**
 * @tc.name      : wcrtomb_0300
 * @tc.desc      : Wide character length exceeds 0x800
 * @tc.level     : Level 1
 */
void wcrtomb_0300(void)
{
    wchar_t wcs = L'\ud10f';
    int want = 3;
    char s[20];
    mbstate_t pr;
    memset(&pr, 0, sizeof pr);
    int result = wcrtomb(s, wcs, &pr);
    if (result != want) {
        t_error("%s wcrtomb get result is %d are not 3\n", __func__, result);
    }
    if (strcmp(s, "턏") != 0) {
        t_error("%s wcrtomb error get s is '%s' are not '턏'\n", __func__, s);
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_US.utf8");

    wcrtomb_0100();
    wcrtomb_0200();
    wcrtomb_0300();
    return t_status;
}
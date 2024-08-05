/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include "functionalext.h"

/**
 * @tc.name      : iswalhpa_0100
 * @tc.desc      : Determine if the input wide character is a letter.
 * @tc.level     : Level 0
 */
void iswalhpa_0100()
{
    setlocale(LC_CTYPE, "UTF-8");
    wchar_t str[] = L"a";
    int ret;
    int i = 0;
    while (str[i]) {
        ret = iswalpha(str[i]);
        if (!ret) {
            wprintf(L"'%lc' is not\n", str[i]);
        }
        i++;
    }
    EXPECT_NE("iswalhpa_0100", ret, 0);
}

/**
 * @tc.name      : iswalhpa_0200
 * @tc.desc      : Determine if the input wide character is a letter.
 * @tc.level     : Level 0
 */
void iswalhpa_0200()
{
    setlocale(LC_CTYPE, "UTF-8");
    wchar_t str[] = L"A";
    int ret;
    int i = 0;
    while (str[i]) {
        ret = iswalpha(str[i]);
        if (!ret) {
            wprintf(L"'%lc' is not\n", str[i]);
        }
        i++;
    }
    EXPECT_NE("iswalhpa_0200", ret, 0);
}

/**
 * @tc.name      : iswalhpa_0300
 * @tc.desc      : Determine if the input wide character is not a letter.
 * @tc.level     : Level 2
 */
void iswalhpa_0300()
{
    setlocale(LC_CTYPE, "UTF-8");
    wchar_t str[] = L"1";
    int ret;
    int i = 0;
    while (str[i]) {
        ret = iswalpha(str[i]);
        if (ret) {
            wprintf(L"'%lc' is alphabetic\n", str[i]);
        }
        i++;
    }
    EXPECT_EQ("iswalhpa_0300", ret, 0);
}

/**
 * @tc.name      : iswalhpa_0400
 * @tc.desc      : Determine if the input wide character is not a letter.
 * @tc.level     : Level 2
 */
void iswalhpa_0400()
{
    setlocale(LC_CTYPE, "UTF-8");
    wchar_t str[] = L"【";
    int ret;
    int i = 0;
    while (str[i]) {
        ret = iswalpha(str[i]);
        if (ret) {
            wprintf(L"'%lc' is alphabetic\n", str[i]);
        }
        i++;
    }
    EXPECT_EQ("iswalhpa_0400", ret, 0);
}

int main(int argc, char *argv[])
{
    iswalhpa_0100();
    iswalhpa_0200();
    iswalhpa_0300();
    iswalhpa_0400();
    return t_status;
}
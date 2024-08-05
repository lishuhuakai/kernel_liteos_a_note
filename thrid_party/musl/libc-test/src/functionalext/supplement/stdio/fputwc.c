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

#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include "functionalext.h"

const int SUCCESS = 0;
const int FAILED = -1;
const int STRING_EQUAL = 0;

/**
 * @tc.name      : fputwc_0100
 * @tc.desc      : Each parameter is valid, a single call can output wide characters to the file stream
 * @tc.level     : Level 0
 */
void fputwc_0100()
{
    wchar_t str[] = L"T";
    wchar_t buf[100];
    wint_t wc;
    int ri = 0;
    int j = 0;
    FILE *fp = fopen("file.txt", "w+");
    if (fp) {
        for (unsigned int i = 0; i < wcslen(str); i++) {
            wc = fputwc(str[i], fp);
            if (wc != str[i]) {
                ri++;
            }
        }
    } else {
        t_error("%s File opening failed", __func__);
    }

    fclose(fp);
    FILE *ffp = fopen("file.txt", "r");
    EXPECT_PTRNE("fputwc_0100", ffp, NULL);
    fgetws(buf, wcslen(str) + 1, ffp);
    EXPECT_EQ("fputwc_0100", ri, 0);
    EXPECT_EQ("fputwc_0100", wcscoll(buf, str), STRING_EQUAL);

    fclose(ffp);
    remove("file.txt");
}

/**
 * @tc.name      : fputwc_0200
 * @tc.desc      : Each parameter is valid, called multiple times, and can output wide characters to the file stream
 * @tc.level     : Level 1
 */
void fputwc_0200()
{
    wchar_t str[] = L"This is a test";
    wchar_t buf[100];
    wint_t wc;
    int ri = 0;
    int j = 0;
    FILE *fp = fopen("file.txt", "w+");
    if (fp) {
        for (unsigned int i = 0; i < wcslen(str); i++) {
            wc = fputwc(str[i], fp);
            if (wc != str[i]) {
                ri++;
            }
        }
    } else {
        t_error("%s File opening failed", __func__);
    }
    fclose(fp);
    FILE *ffp = fopen("file.txt", "r");
    EXPECT_PTRNE("fputwc_0200", ffp, NULL);
    fgetws(buf, wcslen(str) + 1, ffp);
    EXPECT_EQ("fputwc_0200", ri, 0);
    EXPECT_EQ("fputwc_0200", wcscoll(buf, str), STRING_EQUAL);
    fclose(ffp);
    remove("file.txt");
}

int main(int argc, char *argv[])
{
    fputwc_0100();
    fputwc_0200();
    return t_status;
}
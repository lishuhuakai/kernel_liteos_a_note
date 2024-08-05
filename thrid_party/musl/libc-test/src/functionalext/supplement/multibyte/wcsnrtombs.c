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

#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wcsnrtombs_0100
 * @tc.desc      : Convert wn wide characters in a wide string to a multibyte string
 * @tc.level     : Level 0
 */
void wcsnrtombs_0100(void)
{
    const wchar_t src[] = L"test wcsnrtombs";
    const wchar_t *p;
    mbstate_t mbs;
    char buffer[32];
    int wn = 4;
    int n = 4;
    mbrlen(NULL, 0, &mbs);
    memset(buffer, 0, sizeof(buffer));
    p = src;
    int result = wcsnrtombs(buffer, &p, wn, n, &mbs);
    if (result < 0) {
        t_error("%s, get result failed", __func__);
    }
    if (strcmp(buffer, "test")) {
        t_error("%s string in buffer is not right", __func__);
    }
}

/**
 * @tc.name      : wcsnrtombs_0200
 * @tc.desc      : Conversion of wide characters to multibyte characters when wn < n
 * @tc.level     : Level 1
 */
void wcsnrtombs_0200(void)
{
    const wchar_t src[] = L"test wcsnrtombs";
    const wchar_t *p;
    mbstate_t mbs;
    char buffer[32];
    int wn = 4;
    int n = 5;
    mbrlen(NULL, 0, &mbs);
    memset(buffer, 0, sizeof(buffer));
    p = src;
    int result = wcsnrtombs(buffer, &p, wn, n, &mbs);
    if (result < 0) {
        t_error("%s get result failed", __func__);
    }
    if (strcmp(buffer, "test")) {
        t_error("%s string in buffer is not right", __func__);
    }
}

/**
 * @tc.name      : wcsnrtombs_0300
 * @tc.desc      : Conversion of wide characters to multibyte characters when dst=NULL
 * @tc.level     : Level 2
 */
void wcsnrtombs_0300(void)
{
    const wchar_t src[] = L"test wcsnrtombs";
    const wchar_t *p;
    mbstate_t mbs;
    char *buffer = NULL;
    int wn = 4;
    int n = 4;
    mbrlen(NULL, 0, &mbs);
    p = src;
    int result = wcsnrtombs(buffer, &p, wn, n, &mbs);
    if (result < 0) {
        t_error("%s get result failed", __func__);
    }
    if (result != wn) {
        t_error("%s get result is not equal to %d", __func__, wn);
    }
}

int main(int argc, char *argv[])
{
    wcsnrtombs_0100();
    wcsnrtombs_0200();
    wcsnrtombs_0300();
    return t_status;
}
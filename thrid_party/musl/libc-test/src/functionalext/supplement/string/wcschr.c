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
 * @tc.name      : wcschr_0100
 * @tc.desc      : returns a pointer to the first occurrence of the wide character c
 * @tc.level     : Level 0
 */
void wcschr_0100(void)
{
    wchar_t wcs[] = L"This is a c test for wcschr";
    wchar_t *p;
    p = wcschr(wcs, L'c');
    if (*p != L'c') {
        t_error("%s the character pointed to by the pointer is not c", __func__);
    }
}

/**
 * @tc.name      : wcschr_0200
 * @tc.desc      : The target character is empty, returns a pointer to the end of the wide string
 * @tc.level     : Level 1
 */
void wcschr_0200(void)
{
    wchar_t wcs[] = L"This is a c test for wcschr";
    wchar_t *p;
    p = wcschr(wcs, L'\0');
    if ((p - wcs) != wcslen(wcs)) {
        t_error("%s pointer does not point to end of wide string", __func__);
    }
}

/**
 * @tc.name      : wcschr_0300
 * @tc.desc      : The first character of the wide string is the target character
 * @tc.level     : Level 1
 */
void wcschr_0300(void)
{
    wchar_t wcs[] = L"This is a c test for wcschr";
    wchar_t *p;
    p = wcschr(wcs, L'T');
    if (*p != L'T') {
        t_error("%s the character pointed to by the pointer is not T", __func__);
    }
}

int main(int argc, char *argv[])
{
    wcschr_0100();
    wcschr_0200();
    wcschr_0300();
    return t_status;
}
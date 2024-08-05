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
 * @tc.name      : wcsrchr_0100
 * @tc.desc      : Test wcsrchr to get the position of the last occurrence of the target wide character in the wide
 *                 string
 * @tc.level     : Level 0
 */
void wcsrchr_0100(void)
{
    wchar_t *ch = L"hello, world";
    wchar_t *result = wcsrchr(ch, L'h');
    if (wcscmp(result, ch) != 0) {
        t_error("%s wcsrchr get result is %s are not want 'hello, world'\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsrchr_0200
 * @tc.desc      : wcsrchr returns result when testing multiple occurrences of target character in wide string
 * @tc.level     : Level 1
 */
void wcsrchr_0200(void)
{
    wchar_t *ch = L"hello, world";
    wchar_t *result = wcsrchr(ch, L'l');
    if (wcscmp(result, ch + 10) != 0) {
        t_error("%s wcsrchr get result is %s are not want 'ld'\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsrchr_0300
 * @tc.desc      : The test wide string does not contain the target wide character is the wcsrchr result
 * @tc.level     : Level 1
 */
void wcsrchr_0300(void)
{
    wchar_t *ch = L"hello, world";
    wchar_t *result = wcsrchr(ch, L'a');
    if (result) {
        t_error("%s wcsrchr get result is %s are not want ''\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsrchr_0100();
    wcsrchr_0200();
    wcsrchr_0300();
    return t_status;
}
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

#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : wcsrtombs_0100
 * @tc.desc      : Test the result of wcspbrk when the target character is punctuation
 * @tc.level     : Level 0
 */
void wcsrtombs_0100(void)
{
    char str[256] = {0};
    int want = 6;
    const wchar_t *src = L"ABCDEF";
    size_t result = wcsrtombs(str, &src, 10, NULL);
    if (result != want) {
        t_error("%s wcsrtombs error get result is %d are not 6", __func__, result);
    }
}

/**
 * @tc.name      : wcsrtombs_0200
 * @tc.desc      : Test the result of wcsrtombs when the incoming number is less than the length of the wide string
 * @tc.level     : Level 1
 */
void wcsrtombs_0200(void)
{
    char str[256] = {0};
    const wchar_t *src = L"ABCDEF";
    size_t result = wcsrtombs(str, &src, 0, NULL);
    if (result != 0) {
        t_error("%s wcsrtombs error get result is %d are not 0", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsrtombs_0100();
    wcsrtombs_0200();
    return t_status;
}
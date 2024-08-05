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

#include <stdlib.h>
#include <wchar.h>

#include "test.h"

#define BUF_SIZE (40)

/**
 * @tc.name      : wcstombs_0100
 * @tc.desc      : convert a wide-character string to a multibyte string
 * @tc.level     : Level 0
 */
void wcstombs_0100(void)
{
    char buf[BUF_SIZE] = {0};
    wchar_t src[] = L"Hello";

    size_t result = wcstombs(buf, src, BUF_SIZE);
    if (result < 0) {
        t_error("%s failed: wcstombs. result = %zu\n", __func__, result);
    }
}

/**
 * @tc.name      : wcstombs_0200
 * @tc.desc      : convert a wide-character string to a multibyte string with zero size
 * @tc.level     : Level 1
 */
void wcstombs_0200(void)
{
    char buf[BUF_SIZE] = {0};
    wchar_t src[] = L"Hello";

    size_t result = wcstombs(buf, src, 0);
    if (result != 0) {
        t_error("%s failed: wcstombs. result = %zu\n", __func__, result);
    }
}

/**
 * @tc.name      : wcstombs_0300
 * @tc.desc      : convert a wide-character string to NULL
 * @tc.level     : Level 2
 */
void wcstombs_0300(void)
{
    wchar_t src[] = L"Hello";

    size_t result = wcstombs(NULL, src, 0);
    if (result != wcslen(src)) {
        t_error("%s failed: wcstombs. result = %zu\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcstombs_0100();
    wcstombs_0200();
    wcstombs_0300();

    return t_status;
}

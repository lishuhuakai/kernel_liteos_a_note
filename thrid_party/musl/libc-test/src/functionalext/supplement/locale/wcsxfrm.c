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
#include <locale.h>
#include <wchar.h>
#include "test.h"

void wcsxfrm_0100(void)
{
    wchar_t *wc = L"ABCD";
    wchar_t src[20];
    size_t result = wcsxfrm(src, wc, 5);
    if (result != wcslen(wc)) {
        t_error("%s wcsxfrm get result is %d are not %d\n", __func__, result, wcslen(wc));
    }
    if (wcscmp(src, wc) != 0) {
        t_error("%s wcsxfrm get is %ls are not %ls\n", __func__, src, wc);
    }
}

void wcsxfrm_0200(void)
{
    wchar_t *wc = L"\u00e5\ue011";
    wchar_t src[20];
    size_t result = wcsxfrm(src, wc, 1);
    if (result != wcslen(wc)) {
        t_error("%s wcsxfrm get result is %d are not %d\n", __func__, result, wcslen(wc));
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_COLLATE, "en_US.utf8");
    /**
     * @tc.name      : wcsxfrm_0100
     * @tc.desc      : Call wcsxfrm to copy wide strings
     * @tc.level     : Level 0
     */
    wcsxfrm_0100();
    /**
     * @tc.name      : wcsxfrm_0200
     * @tc.desc      : The incoming length is less than the wide string length
     * @tc.level     : Level 0
     */
    wcsxfrm_0200();
    return t_status;
}
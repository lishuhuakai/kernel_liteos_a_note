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

void wcscoll_test(wchar_t *l, wchar_t *r, char *func_name, int want_result)
{
    int result = wcscoll(l, r);
    if (result != want_result) {
        t_error("%s wcscoll error get result is %d not want result %d\n", func_name, result, want_result);
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_COLLATE, "en_US.utf8");
    /**
     * @tc.name      : wcscoll_0100
     * @tc.desc      : Call wcscoll to compare the precedence of wide characters in the current locale
     * @tc.level     : Level 0
     */
    wcscoll_test(L"år", L"ängel", "wcscoll_0100", 1);
    return t_status;
}
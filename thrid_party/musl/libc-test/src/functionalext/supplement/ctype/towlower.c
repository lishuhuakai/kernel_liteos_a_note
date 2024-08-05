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
#include <ctype.h>
#include <wctype.h>
#include "test.h"

/**
 * @tc.name      : towlower_0100
 * @tc.desc      : Convert wide characters to lowercase via towlower
 * @tc.level     : Level 0
 */
void towlower_0100(void)
{
    wint_t wt = L'\u0190';
    wint_t result = towlower(wt);
    unsigned int want = 603;
    if (result != want) {
        t_error("%s towlower get result is %d are not 603\n", __func__, result);
    }
}

/**
 * @tc.name      : towlower_0200
 * @tc.desc      : Test that the incoming character exceeds the function limit
 * @tc.level     : Level 1
 */
void towlower_0200(void)
{
    wint_t wt = 0x20000;
    wint_t result = towlower(wt);
    if (result != wt) {
        t_error("%s towlower get result is %d are not 0x20000\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    towlower_0100();
    towlower_0200();
    return t_status;
}
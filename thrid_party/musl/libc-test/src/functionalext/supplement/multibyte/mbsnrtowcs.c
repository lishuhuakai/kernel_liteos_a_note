/**
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

#include <stdlib.h>
#include <wctype.h>
#include <wchar.h>

#include "functionalext.h"

#define ARRY_MAX 128

/**
 * @tc.name      : mbsnrtowcs_0100
 * @tc.desc      : whether the function successfully converted the character
 * @tc.level     : Level 0
 */
void mbsnrtowcs_0100(void)
{
    const char *test = "musl test";

    wchar_t wc[ARRY_MAX];
    memset(wc, 0, sizeof(wc));

    size_t result = mbsnrtowcs(wc, &test, 2, ARRY_MAX, NULL);
    if (result != -1) {
        EXPECT_EQ("mbsnrtowcs_0100", result, 2);
        EXPECT_EQ("mbsnrtowcs_0100", wc[0], L'm');
        EXPECT_EQ("mbsnrtowcs_0100", wc[1], L'u');
    }
}

/**
 * @tc.name      : mbsnrtowcs_0200
 * @tc.desc      : Pass in the return value of the abnormal length judgment function
 * @tc.level     : Level 2
 */
void mbsnrtowcs_0200(void)
{
    const char *test = "musl test";
    wchar_t wc[ARRY_MAX];
    memset(wc, 0, sizeof(wc));
    size_t result = mbsnrtowcs(wc, &test, 11, ARRY_MAX, NULL);

    EXPECT_EQ("mbsnrtowcs_0200", result, 9);
}

int main(void)
{
    mbsnrtowcs_0100();
    mbsnrtowcs_0200();

    return t_status;
}
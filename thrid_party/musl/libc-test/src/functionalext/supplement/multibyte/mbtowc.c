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
 * @tc.name      : mblen_0100
 * @tc.desc      : whether the function successfully converted the character
 * @tc.level     : Level 0
 */
void mbtowc_0100(void)
{
    const char *test = "musl";
    wchar_t wc[ARRY_MAX];
    size_t result = 0;
    result = mbtowc(wc, test, 1);

    EXPECT_EQ("mbtowc_0100", result, 1);
    EXPECT_EQ("mbtowc_0100", wc[0], L'm');
}

/**
 * @tc.name      : mbtowc_0200
 * @tc.desc      : Pass in the return value of the abnormal length judgment function
 * @tc.level     : Level 2
 */
void mbtowc_0200(void)
{
    const char *test = "musl";
    wchar_t wc[ARRY_MAX];
    size_t result = 0;
    result = mbtowc(wc, test, 0);
    EXPECT_EQ("mbtowc_0200", result, -1);
    EXPECT_EQ("mbtowc_0200", errno, EILSEQ);

    result = mbtowc(wc, NULL, 0);
    EXPECT_EQ("mbtowc_0200", result, 0);
    EXPECT_EQ("mbtowc_0200", errno, EILSEQ);

    result = mbtowc(NULL, test, 1);
    EXPECT_EQ("mbtowc_0200", result, 1);
    EXPECT_EQ("mbtowc_0200", errno, EILSEQ);
}

int main(void)
{
    mbtowc_0100();
    mbtowc_0200();

    return t_status;
}
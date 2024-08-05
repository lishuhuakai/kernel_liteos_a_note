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
#include "functionalext.h"

#define ARRY_MAX 128

/**
 * @tc.name      : mblen_0100
 * @tc.desc      : Verify mblen process success when wide string is not null.
 * @tc.level     : Level 0
 */
void mblen_0100(void)
{
    char *src = (char *)malloc(ARRY_MAX);
    if (!src) {
        return;
    }
    wchar_t *pwc = L"mblen_0100";
    wchar_t *pwcs = (wchar_t *)malloc(sizeof(wchar_t));
    EXPECT_PTRNE("mblen_0100", pwcs, NULL);
    int result = wcstombs(src, pwc, ARRY_MAX);
    EXPECT_NE("mblen_0100", result, -1);
    size_t ret = mblen(src, ARRY_MAX);
    EXPECT_EQ("mblen_0100", ret, 1);
}

/**
 * @tc.name      : mblen_0200
 * @tc.desc      : Verify mblen process success when wide string is null.
 * @tc.leve      : Level 0
 */
void mblen_0200(void)
{
    size_t ret = mblen(NULL, ARRY_MAX);
    EXPECT_EQ("mblen_0100", ret, 0);
}

int main(void)
{
    mblen_0100();
    mblen_0200();
    return t_status;
}
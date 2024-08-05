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

#define TEST_BUFFER_SIZE 128

/**
 * @tc.name      : mbstowcs_0100
 * @tc.desc      : Convert a multibyte string to a wide-character string
 * @tc.level     : Level 0
 */
void mbstowcs_0100(void)
{
    const char *src = "mbstowcs_0100";
    wchar_t dst[TEST_BUFFER_SIZE];
    const wchar_t *wsrc = L"mbstowcs_0100";

    memset(dst, 0x0, sizeof(wchar_t) * TEST_BUFFER_SIZE);
    size_t ret = mbstowcs(dst, src, strlen(src));
    EXPECT_EQ("mbstowcs_0100", ret, strlen(src));

    if (ret > 0) {
        int r = wmemcmp(dst, wsrc, ret);
        EXPECT_EQ("mbstowcs_0100", r, CMPFLAG);
    }
}

/**
 * @tc.name      : mbstowcs_0200
 * @tc.desc      : Provide exception parameter data, convert the string to a wide string
 * @tc.level     : Level 2
 */
void mbstowcs_0200(void)
{
    const char *src = "mbstowcs_0200";
    wchar_t dst[TEST_BUFFER_SIZE];
    memset(dst, 0x0, sizeof(wchar_t) * TEST_BUFFER_SIZE);

    size_t ret = mbstowcs(dst, src, 0);
    if (ret != CMPFLAG) {
        EXPECT_EQ("mbstowcs_0200", ret, CMPFLAG);
    }

    ret = mbstowcs(NULL, src, strlen(src));
    EXPECT_EQ("mbstowcs_0200", ret, strlen(src));
}

int main(void)
{
    mbstowcs_0100();
    mbstowcs_0200();
    return t_status;
}

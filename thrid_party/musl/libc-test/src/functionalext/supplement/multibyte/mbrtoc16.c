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

#include <uchar.h>
#include <string.h>
#include <locale.h>
#include "functionalext.h"

#define TWO_BYTE_TEST_VALUE 0x00a3
#define THREE_BYTE_TEST_VALUE 0x08a3
#define TWO 2
#define THREE 3

/**
 * @tc.name      : mbrtoc16_0100
 * @tc.desc      : Convert a single-byte string to a wide string
 * @tc.level     : Level 0
 */
void mbrtoc16_0100(void)
{
    char16_t pc = 0;
    int ret = -1;
    char *str = setlocale(LC_CTYPE, "C.UTF-8");
    if (str == NULL) {
        t_error("setlocale failed\n");
        return;
    }
    uselocale(LC_GLOBAL_LOCALE);

    ret = mbrtoc16(&pc, "abcdef", 1, NULL);
    EXPECT_EQ("mbrtoc16_0100", ret, 1);
    EXPECT_EQ("mbrtoc16_0100", pc, 'a');
}

/**
 * @tc.name      : mbrtoc16_0200
 * @tc.desc      : Convert a multibyte string to a wide-character string
 * @tc.level     : Level 0
 */
void mbrtoc16_0200(void)
{
    char16_t out;
    int ret = 0;
    char str[] = "\u00a3";
    char str1[] = "\u08a3";
    mbstate_t ps;
    memset(&ps, 0x0, sizeof(ps));

    ret = mbrtoc16(&out, str, strlen(str), NULL);
    EXPECT_EQ("mbrtoc16_0200", ret, TWO);
    EXPECT_EQ("mbrtoc16_0200", out, TWO_BYTE_TEST_VALUE);

    ret = mbrtoc16(&out, str1, strlen(str1), &ps);
    EXPECT_EQ("mbrtoc16_0200", ret, THREE);
    EXPECT_EQ("mbrtoc16_0200", out, THREE_BYTE_TEST_VALUE);
}

/**
 * @tc.name      : mbrtoc16_0300
 * @tc.desc      : Provide exception parameter data, convert the string to a wide string
 * @tc.level     : Level 2
 */
void mbrtoc16_0300(void)
{
    char16_t out;
    char str[] = "\xc2";
    char strx[] = "\xc2\xa3";
    int errorresult = -2;
    mbstate_t ps;
    memset(&ps, 0x0, sizeof(ps));

    int ret = (int)(mbrtoc16(&out, str, sizeof(str), NULL));
    EXPECT_EQ("mbrtoc16_0300", ret, ERREXPECT);
    EXPECT_EQ("mbrtoc16_0300", errno, EILSEQ);
    ret = (int)(mbrtoc16(&out, strx, 1, NULL));
    EXPECT_EQ("mbrtoc16_0300", ret, errorresult);
    ret = (int)(mbrtoc16(NULL, NULL, 0, &ps));
    EXPECT_EQ("mbrtoc16_0300", ret, CMPFLAG);
}

int main(void)
{
    mbrtoc16_0100();
    mbrtoc16_0200();
    mbrtoc16_0300();
    return t_status;
}

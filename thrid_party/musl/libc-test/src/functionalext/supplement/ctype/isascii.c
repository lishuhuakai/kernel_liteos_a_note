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

#include <ctype.h>
#include "functionalext.h"

#define TEST_DATA_COUNT 5

/**
 * @tc.name      : isascii_0100
 * @tc.desc      : Test if character is ascii
 * @tc.level     : Level 0
 */
void isascii_0100(void)
{
    const unsigned char ascii[TEST_DATA_COUNT] = {0x10, 0x20, 0x74, 0x34, 0x7f};
    const unsigned char not_ascii[TEST_DATA_COUNT] = {0x80, 0xef, 0xab, 0x91, 0xfe};
    int i;
    for (i = 0; i < TEST_DATA_COUNT; i++) {
        EXPECT_EQ("isascii_0100", isascii(ascii[i]), ONREXPECT);
    }

    for (i = 0; i < TEST_DATA_COUNT; i++) {
        EXPECT_EQ("isascii_0100", isascii(not_ascii[i]), CMPFLAG);
    }
}

int main(void)
{
    isascii_0100();
    return t_status;
}
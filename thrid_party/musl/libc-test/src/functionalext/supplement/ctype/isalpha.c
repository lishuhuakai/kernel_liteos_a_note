/*
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

const int32_t COUNT_ZERO = 0;

/**
 * @tc.name      : isalpha_0100
 * @tc.desc      : Verify that the entered character is a letter (parameter is 'a')
 * @tc.level     : Level 0
 */
void isalpha_0100(void)
{
    int ret = isalpha('a');
    EXPECT_NE("isalpha_0100", ret, COUNT_ZERO);
}

/**
 * @tc.name      : isalpha_0200
 * @tc.desc      : Verify that the entered character is not a letter (parameter is 'A')
 * @tc.level     : Level 2
 */
void isalpha_0200(void)
{
    int ret = isalpha('A');
    EXPECT_NE("isalpha_0200", ret, COUNT_ZERO);
}

/**
 * @tc.name      : isalpha_0300
 * @tc.desc      : Verify that the entered character is not a letter (parameter is '1')
 * @tc.level     : Level 2
 */
void isalpha_0300(void)
{
    int ret = isspace('1');
    EXPECT_EQ("isalpha_0300", ret, COUNT_ZERO);
}

/**
 * @tc.name      : isalpha_0400
 * @tc.desc      : Verify that the entered character is not a letter (parameter is '【')
 * @tc.level     : Level 2
 */
void isalpha_0400(void)
{
    int ret = isspace('[');
    EXPECT_EQ("isalpha_0400", ret, COUNT_ZERO);
}

/**
 * @tc.name      : isalpha_0500
 * @tc.desc      : Verify the number of letters in the ascii code table
 * @tc.level     : Level 1
 */
void isalpha_0500(void)
{
    int total = 0;
    for (int i = 0; i < 128; i++) {
        int ret = isalpha((char)i);
        if (ret) {
            total++;
        }
    }
    EXPECT_EQ("isalpha_0500", total, 52);
}

int main(void)
{
    isalpha_0100();
    isalpha_0200();
    isalpha_0300();
    isalpha_0400();
    isalpha_0500();
    return t_status;
}
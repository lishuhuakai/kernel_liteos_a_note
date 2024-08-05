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

#include <math.h>
#include <stdlib.h>
#include "functionalext.h"

#define TEST_JRAND_SIZE 3
#define TWO 2
#define THIRTYONE 31

/**
 * @tc.name      : jrand48_0100
 * @tc.desc      : Use [1, 2, 3] as parameters to generate a long integer random number
 * @tc.level     : Level 0
 */
void jrand48_0100(void)
{
    unsigned short s[TEST_JRAND_SIZE] = {1, 2, 3};
    long rev = jrand48(s);
    EXPECT_TRUE("jrand48_0100", rev < pow(TWO, THIRTYONE) && rev > -pow(TWO, THIRTYONE));
}

/**
 * @tc.name      : jrand48_0200
 * @tc.desc      : Use [0, 0, 0] as parameters to generate a long integer random number
 * @tc.level     : Level 0
 */
void jrand48_0200(void)
{
    unsigned short s[TEST_JRAND_SIZE] = {0, 0, 0};
    long rev = jrand48(s);
    EXPECT_EQ("jrand48_0200", rev, 0);
}

/**
 * @tc.name      : mrand48_0100
 * @tc.desc      : Verify mrand48 process success. Generate a long integer random number
 * @tc.level     : Level 0
 */
void mrand48_0100(void)
{
    unsigned short p[7] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090a, 0x0b0c, 0x0d0e};
    lcong48(p);
    long ret = mrand48();
    EXPECT_TRUE("mrand48_0100", ret < pow(TWO, THIRTYONE) && ret > -pow(TWO, THIRTYONE));
}

int main(void)
{
    mrand48_0100();
    jrand48_0100();
    jrand48_0200();
    return t_status;
}
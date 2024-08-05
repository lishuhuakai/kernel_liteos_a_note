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

#define TEST_NRAND_SIZE 3
#define TEST_TIMES 10

/**
 * @tc.name      : nrand48_0100
 * @tc.desc      : Generate a random number of 10 long integers
 * @tc.level     : Level 0
 */
void nrand48_0100(void)
{
    unsigned short s[TEST_NRAND_SIZE] = {1, 2, 3};
    int i, j;
    for (i = 0; i < TEST_TIMES; i++) {
        for (j = 0; j < TEST_NRAND_SIZE; j++) {
            s[j] += i * TEST_TIMES;
        }
        long rev = nrand48(s);
        EXPECT_TRUE("nrand48_0100", rev < pow(2, 31) && rev >= 0);
    }
}

int main(void)
{
    nrand48_0100();
    return t_status;
}
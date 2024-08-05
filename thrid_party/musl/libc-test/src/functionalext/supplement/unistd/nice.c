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

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

#define TEST_NICE_RAT (-2)

/**
 * @tc.name      : msync_0100
 * @tc.desc      : Modified scheduling priority
 * @tc.level     : Level 0
 */
void nice_0100(void)
{
    int rev = -1;
    rev = nice(NZERO);
    EXPECT_TRUE("nice_0100", rev > 0);

    rev = nice((TEST_NICE_RAT)*NZERO);
    EXPECT_EQ("nice_0100", rev, -NZERO);
}

/**
 * @tc.name      : nice_0200
 * @tc.desc      : Exception parameters, modify scheduling priority
 * @tc.level     : Level 2
 */
void nice_0200(void)
{
    int rev = nice(NZERO - 1);
    EXPECT_EQ("nice_0200", rev, -1);
}

int main(void)
{
    nice_0100();
    nice_0200();
    return t_status;
}
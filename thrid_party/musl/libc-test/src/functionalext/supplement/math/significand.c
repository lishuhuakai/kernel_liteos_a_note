/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <float.h>
#include <math.h>
#include "test.h"

#define EXPECT_DOUBLE_EQ(a, b)                                             \
    do {                                                                   \
        if (!(fabs(a - b) < DBL_EPSILON))                                  \
            t_error("%s failed: %f is not equal to %f\n", __func__, a, b); \
    } while (0)

/*
 * @tc.name      : significand_0100
 * @tc.desc      : Get mantissa of floating-point number 0.0
 * @tc.level     : Level 0
 */
void significand_0100(void)
{
    double d = significand(0.0);
    EXPECT_DOUBLE_EQ(0.0, d);
}

/*
 * @tc.name      : significand_0200
 * @tc.desc      : Get mantissa of floating-point number 1.2
 * @tc.level     : Level 1
 */
void significand_0200(void)
{
    double d = significand(1.2);
    EXPECT_DOUBLE_EQ(1.2, d);
}

/*
 * @tc.name      : significand_0300
 * @tc.desc      : Get mantissa of floating-point number 12.25
 * @tc.level     : Level 1
 */
void significand_0300(void)
{
    double d = significand(12.25);
    EXPECT_DOUBLE_EQ(1.53125, d);
}

int main(int argc, char *argv[])
{
    significand_0100();
    significand_0200();
    significand_0300();
    return t_status;
}
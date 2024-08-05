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

const int TWO = 2;
const int THRITYONE = 31;

/**
 * @tc.name      : lcong48_0100
 * @tc.desc      : Verify lcong48 process success. Set random number seed for 48 bit operation.
 * @tc.level     : Level 0
 */
void lcong48_0100(void)
{
    unsigned short p[7] = {0x0102, 0x0304, 0x0506, 0x0708, 0x090a, 0x0b0c, 0x0d0e};
    lcong48(p);
    long ret = mrand48();
    EXPECT_TRUE("lcong48_0100", ret < pow(TWO, THRITYONE) && ret > -pow(TWO, THRITYONE));
}

int main(void)
{
    lcong48_0100();
    return t_status;
}
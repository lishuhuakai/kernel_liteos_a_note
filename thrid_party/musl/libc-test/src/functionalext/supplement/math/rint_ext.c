/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <math.h>
#include <stdlib.h>
#include "functionalext.h"

const float argNum1 = 2.4;
const float argNum2 = 3.9;
const float argNum3 = -2.4;
const float argNum4 = -3.9;

const int32_t retNum1 = 2;
const int32_t retNum2 = 4;
const int32_t retNum3 = -2;
const int32_t retNum4 = -4;

/**
 * @tc.name      : rint_0100
 * @tc.desc      : Verify rint process success and return 0 when param is 2.4
 * @tc.level     : Level 0
 */
void rint_0100(void)
{
    int32_t ret = rint(argNum1);
    EXPECT_EQ("rint_0100", ret, retNum1);
}

/**
 * @tc.name      : rint_0200
 * @tc.desc      : Verify rint process success and return 0 when param is 3.9
 * @tc.level     : Level 0
 */
void rint_0200(void)
{
    int32_t ret = rint(argNum2);
    EXPECT_EQ("rint_0200", ret, retNum2);
}

/**
 * @tc.name      : rint_0300
 * @tc.desc      : Verify rint process success and return 0 when param is -2.4
 * @tc.level     : Level 0
 */
void rint_0300(void)
{
    int32_t ret = rint(argNum3);
    EXPECT_EQ("rint_0300", ret, retNum3);
}

/**
 * @tc.name      : rint_0400
 * @tc.desc      : Verify rint process success and return 0 when param is -3.9
 * @tc.level     : Level 0
 */
void rint_0400(void)
{
    int32_t ret = rint(argNum4);
    EXPECT_EQ("rint_0300", ret, retNum4);
}

/**
 * @tc.name      : rint_0500
 * @tc.desc      : Verify rint process success and return 0 when param is 0
 * @tc.level     : Level 0
 */
void rint_0500(void)
{
    int32_t ret = rint(0);
    EXPECT_EQ("rint_0400", ret, 0);
}

int main(void)
{
    rint_0100();
    rint_0200();
    rint_0300();
    rint_0400();
    rint_0500();
    return t_status;
}

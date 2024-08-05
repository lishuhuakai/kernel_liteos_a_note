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

#include <float.h>
#include <math.h>
#include "functionalext.h"

/**
 * @tc.name      : __fpclassifyf_0100
 * @tc.desc      : The parameter is valid, x is 1.0, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 0
 */
void __fpclassifyf_0100(void)
{
    float x = 1.0;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0100", ret, FP_NORMAL);
}

/**
 * @tc.name      : __fpclassifyf_0200
 * @tc.desc      : The parameter is valid, x is NAN, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyf_0200(void)
{
    float x = NAN;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0200", ret, FP_NAN);
}

/**
 * @tc.name      : __fpclassifyf_0300
 * @tc.desc      : The parameter is valid, x is INFINITY, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyf_0300(void)
{
    float x = INFINITY;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0300", ret, FP_INFINITE);
}

/**
 * @tc.name      : __fpclassifyf_0400
 * @tc.desc      : The parameter is valid, x is -0.0, can get the integer value matching the
 *                 classification macro constant.
 * @tc.level     : Level 1
 */
void __fpclassifyf_0400(void)
{
    float x = -0.0;
    int ret = __fpclassifyf(x);
    EXPECT_EQ("__fpclassifyf_0400", ret, FP_ZERO);
}

int main(int argc, char *argv[])
{
    __fpclassifyf_0100();
    __fpclassifyf_0200();
    __fpclassifyf_0300();
    __fpclassifyf_0400();
    return t_status;
}
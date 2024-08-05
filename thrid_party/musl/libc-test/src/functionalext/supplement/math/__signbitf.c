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

#include <math.h>
#include "functionalext.h"

/*
 * @tc.name      :  __signbitf_0100
 * @tc.desc      : The parameter value of float type is 0, and the sign bit of the return parameter is 0.
 * @tc.level     : Level 0
 */
void __signbitf_0100(void)
{
    float x = 0.0f;
    int result = __signbitf(x);
    EXPECT_EQ("__signbitf_0100", result, 0);
}

/*
 * @tc.name      :  __signbitf_0200
 * @tc.desc      : The parameter value of type float is a positive number, and the sign bit of the returned
 *                 parameter is 0.
 * @tc.level     : Level 0
 */
void __signbitf_0200(void)
{
    float x = 3.14f;
    int result = __signbitf(x);
    EXPECT_EQ("__signbitf_0200", result, 0);
}

/*
 * @tc.name      :  __signbitf_0300
 * @tc.desc      : The parameter value of type float is negative,
 *                 and the sign bit of the return parameter is 1.
 * @tc.level     : Level 0
 */
void __signbitf_0300(void)
{
    float x = -3.14f;
    int result = __signbitf(x);
    EXPECT_EQ("__signbitf_0300", result, 1);
}

int main(int argc, char *argv[])
{
    __signbitf_0100();
    __signbitf_0200();
    __signbitf_0300();
    return t_status;
}
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

typedef void (*TEST_FUN)();
const int32_t COUNT_ZERO = 0;
const int32_t COUNT_ONE = 1;

/**
 * @tc.name      : __signbit_0100
 * @tc.desc      : When the float value is 0, test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_0100()
{
    double x = 0;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0100", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_0200
 * @tc.desc      : When the float value is 3.14, test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_0200()
{
    double x = 3.14;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0200", result, COUNT_ZERO);
}

/**
 * @tc.name      : __signbit_0300
 * @tc.desc      : When the float value is -3.14, test the return value of the function
 * @tc.level     : Level 0
 */
void __signbit_0300()
{
    double x = -3.14;
    int result = __signbit(x);
    EXPECT_EQ("_signbit_0300", result, COUNT_ONE);
}

TEST_FUN G_Fun_Array[] = {
    __signbit_0100,
    __signbit_0200,
    __signbit_0300,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
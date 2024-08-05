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
#include <stdlib.h>
#include "test.h"

typedef void (*TEST_FUN)();

#define EXPECT_DOUBLE_EQ(a, b)                                             \
    do {                                                                   \
        if (!(fabs(a - b) < DBL_EPSILON))                                  \
            t_error("%s failed: %f is not equal to %f\n", __func__, a, b); \
    } while (0)

/**
 * @tc.name      : atof_0100
 * @tc.desc      : Convert a string to a double
 * @tc.level     : Level 0
 */
void atof_0100()
{
    const char *str = "123456.00";
    double result = atof(str);
    EXPECT_DOUBLE_EQ(result, 123456.00);
}

TEST_FUN G_Fun_Array[] = {
    atof_0100,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
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

#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : srand48_0100
 * @tc.desc      : Verify that the random number can be obtained by calling the srand48 function.
 * @tc.level     : Level 0
 */
void srand48_0100(void)
{
    struct srand48_data {
        long seed;
        double value[5];
    };
    struct srand48_data test_data[2] = {
        {1,
            {0.0416303447718782138,
                0.454492444728629152,
                0.834817218166914898,
                0.335986030145200232,
                0.565489403566136417}},
        {2,
            {0.912432653437466712,
                0.15908290897229449,
                0.573262780710262376,
                0.801506833263324836,
                0.553675300337165055}},
    };
    int i, j;
    for (i = 0; i < 2; i++) {
        srand48(test_data[i].seed);
        for (j = 0; j < 5; j++) {
            EXPECT_TRUE("srand48_0100", fabs(drand48() - test_data[i].value[j]) < 0.00000000001);
        }
    }
}

int main(int argc, char *argv[])
{
    srand48_0100();

    return t_status;
}
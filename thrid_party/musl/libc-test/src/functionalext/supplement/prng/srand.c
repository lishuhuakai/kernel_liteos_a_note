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

#include <stdio.h>
#include <stdlib.h>
#include "test.h"

/**
 * @tc.name      : srand_0100
 * @tc.desc      : Test the srand function to set the random number seed
 * @tc.level     : Level 0
 */
void srand_0100(void)
{
    int a[10] = {0};
    int b[10] = {0};
    srand(10);
    for (int i = 0; i < 10; i++) {
        a[i] = rand();
    }
    srand(10);
    for (int i = 0; i < 10; i++) {
        b[i] = rand();
    }
    for (int i = 0; i < 10; i++) {
        if (a[i] != b[i]) {
            t_error("%s srand rand in %d is %d , %d \n", __func__, i, a[i], b[i]);
        }
    }
}

/**
 * @tc.name      : srand_0200
 * @tc.desc      : Change the random number seed
 * @tc.level     : Level 1
 */
void srand_0200(void)
{
    int a = 0;
    int b = 0;
    srand(10);
    a = rand();
    srand(500);
    b = rand();
    if (a == b) {
        t_error("%s srand get rand is %d , %d \n", __func__, a, b);
    }
}

int main(int argc, char *argv[])
{
    srand_0100();
    srand_0200();
    return t_status;
}
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

unsigned int seed_ = 123;

/**
 * @tc.name      : rand_r_0100
 * @tc.desc      : The return value obtained by calling rand_d is different when testing different random number seeds
 * @tc.level     : Level 0
 */
void rand_r_0100(void)
{
    unsigned int seed = 10;
    int a_result = rand_r(&seed);
    seed = 200;
    int b_result = rand_r(&seed);
    if (a_result == b_result) {
        t_error("%s rand_r error change seed get result is all %d\n", __func__, a_result);
    }
}

/**
 * @tc.name      : rand_r_0200
 * @tc.desc      : When the test changes the same random number seed, the return value obtained by calling rand_d is
 *                 the same
 * @tc.level     : Level 1
 */
void rand_r_0200(void)
{
    unsigned int seed = seed_;
    int a_result = rand_r(&seed);
    seed = seed_;
    int b_result = rand_r(&seed);
    if (a_result != b_result) {
        t_error("%s rand_r error get result is %d and %d \n", __func__, a_result, b_result);
    }
}

/**
 * @tc.name      : rand_r_0300
 * @tc.desc      : Test that the function return value obtained by calling the rand_d function twice in a row is
 *                 different
 * @tc.level     : Level 1
 */
void rand_r_0300(void)
{
    unsigned int seed = 10;
    int a_result = rand_r(&seed);
    int b_result = rand_r(&seed);
    if (a_result == b_result) {
        t_error("%s rand_r error change seed get result is all %d\n", __func__, a_result);
    }
}

int main(int argc, char *argv[])
{
    rand_r_0100();
    rand_r_0200();
    rand_r_0300();
    return t_status;
}
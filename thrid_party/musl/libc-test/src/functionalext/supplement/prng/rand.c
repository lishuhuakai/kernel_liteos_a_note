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

#include <stdlib.h>
#include <time.h>
#include "functionalext.h"

const int32_t SIZE = 5;
const int32_t RANGE = 10;

/**
 * @tc.name      : rand_0100
 * @tc.desc      : Verify rand process success
 * @tc.level     : Level 0
 */
void rand_0100(void)
{
    srand((unsigned int)time(0));
    int i = 0;
    for (; i < SIZE; ++i) {
        int num = rand() % RANGE;
        EXPECT_LT("rand_0100", num, RANGE);
    }
}

int main(void)
{
    rand_0100();
    return t_status;
}

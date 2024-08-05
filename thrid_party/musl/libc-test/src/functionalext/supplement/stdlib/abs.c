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

#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : abs_0100
 * @tc.desc      : Verify that the absolute value of the returned parameter is 0 (parameter is 0).
 * @tc.level     : Level 0
 */
void abs_0100(void)
{
    int result = abs(0);
    EXPECT_EQ("abs_0100", result, 0);
}

/**
 * @tc.name      : abs_0200
 * @tc.desc      : Verify that the absolute value of the returned parameter is a positive number (parameter is 10).
 * @tc.level     : Level 0
 */
void abs_0200(void)
{
    int result = abs(10);
    EXPECT_EQ("abs_0200", result, 10);
}

/**
 * @tc.name      : abs_0300
 * @tc.desc      : Verify that the absolute value of the returned parameter is a positive number (parameter is -10).
 * @tc.level     : Level 0
 */
void abs_0300(void)
{
    int result = abs(-10);
    EXPECT_EQ("abs_0300", result, 10);
}

int main(int argc, char *argv[])
{
    abs_0100();
    abs_0200();
    abs_0300();
    return t_status;
}
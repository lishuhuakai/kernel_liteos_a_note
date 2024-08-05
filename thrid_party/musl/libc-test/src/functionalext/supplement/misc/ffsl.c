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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <syslog.h>
#include <stdint.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : ffsl_0100
 * @tc.desc      : Validates the ability to find the first set value of a value of type long.
 * @tc.level     : Level 0
 */
void ffsl_0100(void)
{
    int result;
    result = ffsl(0);
    EXPECT_EQ("ffsl_0100", result, 0);
}

/**
 * @tc.name      : ffsl_0200
 * @tc.desc      : Verify to find the first set value of a value of type long (0x8, bit 1)
 * @tc.level     : Level 0
 */
void ffsl_0200(void)
{
    int result;
    result = ffsl(0x8);
    EXPECT_EQ("ffsl_0200", result, 4);
}

/**
 * @tc.name      : ffsl_0300
 * @tc.desc      : Validates the ability to find the first set value of a value of type long (0x8000, bit 1)
 * @tc.level     : Level 0
 */
void ffsl_0300(void)
{
    int result;
    result = ffsl(0x8000);
    EXPECT_EQ("ffsl_0300", result, 16);
}

int main(int argc, char *argv[])
{
    ffsl_0100();
    ffsl_0200();
    ffsl_0300();
    return t_status;
}
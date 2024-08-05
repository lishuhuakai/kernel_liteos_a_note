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

#include <ulimit.h>
#include <stdlib.h>
#include "functionalext.h"

#define MAX_FILE_SIZE 4096

/**
 * @tc.name      : ulimit_0100
 * @tc.desc      : Test ulimit() with UL_SETFSIZE
 * @tc.level     : Level 0
 */
static void ulimit_0100(void)
{
    int cmd = UL_SETFSIZE;
    long result = ulimit(cmd, MAX_FILE_SIZE);
    if (result < 0) {
        EXPECT_PTRNE("ulimit_0100", result, 0);
        return;
    }

    EXPECT_LONGEQ("ulimit_0100", MAX_FILE_SIZE, result);
}

/**
 * @tc.name      : ulimit_0200
 * @tc.desc      : Test ulimit() with UL_GETFSIZE
 * @tc.level     : Level 1
 */
static void ulimit_0200(void)
{
    int cmd = UL_GETFSIZE;
    long result = ulimit(cmd);
    if (result < 0) {
        EXPECT_PTRNE("ulimit_0200", result, 0);
        return;
    }

    EXPECT_LONGEQ("ulimit_0200", MAX_FILE_SIZE, result);
}

/**
 * @tc.name      : ulimit_0300
 * @tc.desc      : Test ulimit() with UL_SETFSIZE and invalid args
 * @tc.level     : Level 2
 */
static void ulimit_0300(void)
{
    int cmd = UL_SETFSIZE;
    long result = ulimit(cmd, -1);
    if (result >= 0) {
        EXPECT_PTRNE("ulimit_0300", result, 0);
        return;
    }

    EXPECT_LONGEQ("ulimit_0300", -1, result);
}

int main(void)
{
    ulimit_0100();
    ulimit_0200();
    ulimit_0300();
    return t_status;
}
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

#include <sys/membarrier.h>
#include "functionalext.h"

/**
 * @tc.name      : membarrier_0100
 * @tc.desc      : Verify memory masking
 *                 (all parameters are valid, CMD is not equal to MEMBARRIER_CMD_PRIVATE_EXPEDITED)
 * @tc.level     : Level 0
 */
void membarrier_0100(void)
{
    int result = membarrier(MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED, 0);
    EXPECT_EQ("membarrier_0100", result, 0);
}

/**
 * @tc.name      : membarrier_0200
 * @tc.desc      : Verify memory masking (valid with CMD equal to MEMBARRIER_CMD_PRIVATE_EXPEDITED)
 * @tc.level     : Level 1
 */
void membarrier_0200(void)
{
    int result = membarrier(MEMBARRIER_CMD_PRIVATE_EXPEDITED, 0);
    EXPECT_EQ("membarrier_0200", result, 0);
}

/**
 * @tc.name      : membarrier_0300
 * @tc.desc      : Verify that memory masking is not possible (CMD argument is invalid)
 * @tc.level     : Level 2
 */
void membarrier_0300(void)
{
    int result = membarrier(-1, 0);
    EXPECT_EQ("membarrier_0300", result, -1);
}

int main(int argc, char *argv[])
{
    membarrier_0100();
    membarrier_0200();
    membarrier_0300();
    return t_status;
}
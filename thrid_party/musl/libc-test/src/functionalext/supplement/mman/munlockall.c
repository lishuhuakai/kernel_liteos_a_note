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
#include <sys/mman.h>
#include "functionalext.h"

/**
 * @tc.name      : munlockall_0100
 * @tc.desc      : Verify munlockall success. When lock all flag is MCL_CURRENT.
 * @tc.level     : Level 0
 */
void munlockall_0100(void)
{
    size_t memsize = 4096;
    char *memory = (char *)malloc(memsize);
    if (memory == NULL) {
        EXPECT_PTRNE("munlockall_0100", memory, NULL);
        return;
    }
    memset(memory, '\0', memsize);
    int result = mlockall(MCL_CURRENT);
    EXPECT_EQ("munlockall_0100", result, 0);

    result = munlockall();
    EXPECT_EQ("munlockall_0100", result, 0);
    free(memory);
    memory = NULL;
}

/**
 * @tc.name      : munlockall_0200
 * @tc.desc      : Verify munlockall success. When lock all flag is MCL_CURRENT.
 * @tc.level     : Level 0
 */
void munlockall_0200(void)
{
    int result = mlockall(MCL_FUTURE);
    EXPECT_EQ("munlockall_0200", result, 0);
    size_t memsize = 4096;
    char *memory = (char *)malloc(memsize);
    if (memory == NULL) {
        EXPECT_PTRNE("munlockall_0200", memory, NULL);
        return;
    }
    memset(memory, '\0', memsize);
    result = munlockall();
    EXPECT_EQ("munlockall_0200", result, 0);
    free(memory);
    memory = NULL;
}

int main(void)
{
    munlockall_0100();
    munlockall_0200();
    return t_status;
}
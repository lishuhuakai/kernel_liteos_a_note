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
 * @tc.name      : mlockall_0100
 * @tc.desc      : Verify mlockall success. When flag is MCL_CURRENT.
 * @tc.level     : Level 0
 */
void mlockall_0100(void)
{
    size_t memsize = 4096;
    char *memory = (char *)malloc(memsize);
    if (memory == NULL) {
        EXPECT_PTRNE("mlockall_0100", memory, NULL);
        return;
    }
    memset(memory, '\0', memsize);
    int result = mlockall(MCL_CURRENT);
    EXPECT_EQ("mlockall_0100", result, 0);

    munlockall();
    free(memory);
    memory = NULL;
}

/**
 * @tc.name      : mlockall_0200
 * @tc.desc      : Verify mlockall success. When flag is MCL_FUTURE.
 * @tc.level     : Level 0
 */
void mlockall_0200(void)
{
    int result = mlockall(MCL_FUTURE);
    EXPECT_EQ("mlockall_0200", result, 0);
    size_t memsize = 4096;
    char *memory = (char *)malloc(memsize);
    if (memory == NULL) {
        EXPECT_PTRNE("mlockall_0200", memory, NULL);
        return;
    }
    memset(memory, '\0', memsize);
    munlockall();
    free(memory);
    memory = NULL;
}

/**
 * @tc.name      : mlockall_0300
 * @tc.desc      : Verify mlockall success. When flag is error.
 * @tc.level     : Level 0
 */
void mlockall_0300(void)
{
    size_t memsize = 4096;
    char *memory = (char *)malloc(memsize);
    if (memory == NULL) {
        EXPECT_PTRNE("mlockall_0300", memory, NULL);
        return;
    }
    memset(memory, '\0', memsize);
    int result = mlockall(-1);
    EXPECT_EQ("mlockall_0300", result, ERREXPECT);

    free(memory);
    memory = NULL;
}

int main(void)
{
    mlockall_0100();
    mlockall_0200();
    mlockall_0300();
    return t_status;
}
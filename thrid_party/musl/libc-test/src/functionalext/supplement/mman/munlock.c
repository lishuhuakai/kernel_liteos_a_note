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

#define TEST_SIZE 4096

/**
 * @tc.name      : munlock_0100
 * @tc.desc      : Unlock locked memory block
 * @tc.level     : Level 0
 */
void munlock_0100(void)
{
    char *memory = (char *)malloc(TEST_SIZE);
    if (memory == NULL) {
        EXPECT_PTRNE("munlock_0100", memory, NULL);
        return;
    }

    memset(memory, 0x0, TEST_SIZE);
    int ret = mlock(memory, TEST_SIZE);
    EXPECT_NE("munlock_0100", ret, ERREXPECT);

    ret = munlock(memory, TEST_SIZE);
    EXPECT_EQ("munlock_0100", ret, CMPFLAG);
    free(memory);
    memory = NULL;
}

int main(void)
{
    munlock_0100();
    return t_status;
}
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

#include <sys/mman.h>
#include <stdlib.h>

#include "functionalext.h"

/**
 * @tc.name      : mlock_0100
 * @tc.desc      : Assert whether locking 4k memory is successful, release resources after use
 * @tc.level     : Level 0
 */
void mlock_0100(void)
{
    size_t memsize = 4096;
    char *memory = (char *)malloc(memsize);
    if (memory == NULL) {
        EXPECT_PTRNE("mlock_0100", memory, NULL);
        return;
    }
    memset(memory, '\0', memsize);
    int result = mlock(memory, memsize);
    EXPECT_NE("mlock_0100", result, ERREXPECT);

    munlock(memory, memsize);
    free(memory);
    memory = NULL;
}

int main(void)
{
    mlock_0100();

    return t_status;
}
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
 * @tc.name      : mlock2_0100
 * @tc.desc      : Lock 4k memory, release memory after unlocking
 * @tc.level     : Level 0
 */
void mlock2_0100(void)
{
    size_t memsize = 4096;
    char *memory = (char *)malloc(memsize);
    if (memory == NULL) {
        EXPECT_PTRNE("mlock2_0100", memory, NULL);
        return;
    }
    memset(memory, '\0', memsize);
    int ret = mlock2(memory, memsize, CMPFLAG);
    EXPECT_EQ("mlock2_0100", ret, CMPFLAG);

    munlock(memory, memsize);
    free(memory);
    memory = NULL;
}

/**
 * @tc.name      : mlock2_0200
 * @tc.desc      : Lock 1k memory and mark all 4k memory size, release memory after unlocking
 * @tc.level     : Level 0
 */
void mlock2_0200(void)
{
    size_t memsize = 4096;
    size_t memchip = 1024;
    char *memory = (char *)malloc(memsize);
    if (memory == NULL) {
        EXPECT_PTRNE("mlock2_0200", memory, NULL);
        return;
    }
    memset(memory, '\0', memsize);
    int ret = mlock2(memory, memchip, MLOCK_ONFAULT);
    EXPECT_EQ("mlock2_0200", ret, CMPFLAG);

    munlock(memory, memsize);
    free(memory);
    memory = NULL;
}

int main(void)
{
    mlock2_0100();
    mlock2_0200();
    return t_status;
}

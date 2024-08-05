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

#include <errno.h>
#include <stdlib.h>
#include "functionalext.h"

#define TEST_MEMALIGN_ALIGN 16

/**
 * @tc.name      : posix_memalign_0100
 * @tc.desc      : Allocate memory blocks with 16-byte alignment
 * @tc.level     : Level 0
 */
void posix_memalign_0100(void)
{
    size_t block[] = {1, 4, 8, 256, 1024, 65000, 128000, 256000, 1000000};
    int count = sizeof(block) / sizeof(size_t);
    int i = 0;
    for (i = 0; i < count; i++) {
        void *memory = NULL;
        int rev = posix_memalign(&memory, TEST_MEMALIGN_ALIGN, block[i]);
        EXPECT_EQ("posix_memalign_0100", rev, 0);
        EXPECT_TRUE("posix_memalign_0100", memory != NULL);
        if (memory) {
            free(memory);
        }
    }
}

/**
 * @tc.name      : posix_memalign_0200
 * @tc.desc      : Synchronize a file with a memory map
 * @tc.level     : Level 2
 */
void posix_memalign_0200(void)
{
    void *memory = NULL;
    int rev = posix_memalign(&memory, 1, 1);
    EXPECT_EQ("posix_memalign_0200", rev, EINVAL);
    if (memory) {
        free(memory);
    }
}

int main(void)
{
    posix_memalign_0100();
    posix_memalign_0200();
    return t_status;
}
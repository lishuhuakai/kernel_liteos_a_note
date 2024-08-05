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
#include <sys/mman.h>
#include "functionalext.h"

#define TEST_MS_SIZE 1024

/**
 * @tc.name      : msync_0100
 * @tc.desc      : Synchronize a file with a memory map
 * @tc.level     : Level 0
 */
void msync_0100(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("msync_0100", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    if (fd == -1) {
        fclose(fp);
        EXPECT_TRUE("msync_0100", fd != -1);
        return;
    }
    void *map = mmap(NULL, TEST_MS_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (!map) {
        EXPECT_PTRNE("msync_0100", map, NULL);
        return;
    }
    int rev = msync(map, TEST_MS_SIZE, MS_ASYNC);
    EXPECT_EQ("msync_0100", rev, 0);
    rev = munmap(map, TEST_MS_SIZE);
    EXPECT_EQ("msync_0100", rev, 0);
    rev = fclose(fp);
    EXPECT_EQ("msync_0100", rev, 0);
}

int main(void)
{
    msync_0100();
    return t_status;
}
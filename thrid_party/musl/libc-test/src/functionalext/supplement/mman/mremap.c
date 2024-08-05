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

#define TEST_M_SIZE 1024
#define TEST_M_NEW_SIZE 2048

/**
 * @tc.name      : mremap_0100
 * @tc.desc      : Modify the size of the memory map
 * @tc.level     : Level 0
 */
void mremap_0100(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("mremap_0100", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    if (fd == -1) {
        fclose(fp);
        EXPECT_TRUE("mremap_0100", fd != -1);
        return;
    }
    void *map = mmap(NULL, TEST_M_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (!map) {
        EXPECT_PTRNE("mremap_0100", map, NULL);
        return;
    }
    void *map_new = mremap(map, TEST_M_SIZE, TEST_M_NEW_SIZE, MREMAP_MAYMOVE);
    EXPECT_PTREQ("mremap_0100", map_new, map);
    int rev = munmap(map_new, TEST_M_NEW_SIZE);
    EXPECT_EQ("mremap_0100", rev, 0);
    rev = fclose(fp);
    EXPECT_EQ("mremap_0100", rev, 0);
}

/**
 * @tc.name      : mremap_0200
 * @tc.desc      : Modify the size of the memory map with the maximum value, failed
 * @tc.level     : Level 2
 */
void mremap_0200(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("mremap_0200", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    if (fd == -1) {
        fclose(fp);
        EXPECT_TRUE("mremap_0200", fd != -1);
        return;
    }

    void *map = mmap(NULL, TEST_M_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (!map) {
        EXPECT_PTRNE("mremap_0200", map, NULL);
        return;
    }
    void *map_new = mremap(map, TEST_M_SIZE, PTRDIFF_MAX, MREMAP_MAYMOVE);
    EXPECT_PTRNE("mremap_0200", map_new, map);
    int rev = munmap(map, TEST_M_SIZE);
    EXPECT_EQ("mremap_0200", rev, 0);
    rev = fclose(fp);
    EXPECT_EQ("mremap_0200", rev, 0);
}

int main(void)
{
    mremap_0100();
    mremap_0200();
    return t_status;
}
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
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "functionalext.h"
#include "filepath_util.h"

#define TEST_SIZE 4096

/**
 * @tc.name      : mincore_0100
 * @tc.desc      : Determine whether pages are resident in memory
 * @tc.level     : Level 0
 */
void mincore_0100(void)
{
    struct stat st;
    char pathname[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("mincore", pathname);

    int ret = stat(pathname, &st);
    EXPECT_EQ("mincore_0100", ret, CMPFLAG);
    if (ret != 0) {
        return;
    }

    int fd = open(pathname, O_RDONLY);
    EXPECT_NE("mincore_0100", fd, ERREXPECT);
    if (fd == -1) {
        return;
    }

    void *start = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    EXPECT_PTRNE("mincore_0100", start, NULL);
    if (!start) {
        return;
    }

    unsigned char vec[TEST_SIZE];
    memset(vec, 0x0, sizeof(vec));
    ret = mincore(start, TEST_SIZE, vec);
    EXPECT_EQ("mincore_0100", ret, CMPFLAG);

    munmap(start, st.st_size);
}

/**
 * @tc.name      : mincore_0200
 * @tc.desc      : Determine whether pages are resident in memory
 * @tc.level     : Level 2
 */
void mincore_0200(void)
{
    char *memory = (char *)malloc(TEST_SIZE);
    if (memory == NULL) {
        EXPECT_PTRNE("mincore_0200", memory, NULL);
        return;
    }

    unsigned char vec[TEST_SIZE];
    memset(vec, 0x0, sizeof(vec));
    int ret = mincore(memory, TEST_SIZE, vec);
    EXPECT_EQ("mincore_0200", ret, ERREXPECT);

    ret = mincore(memory, TEST_SIZE, NULL);
    EXPECT_EQ("mincore_0200", ret, ERREXPECT);
    free(memory);
    memory = NULL;
}

int main(void)
{
    mincore_0100();
    mincore_0200();
    return t_status;
}
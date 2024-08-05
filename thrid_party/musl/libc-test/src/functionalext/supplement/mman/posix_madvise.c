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

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include "functionalext.h"

const char *path = "posix_madvise_function_test.c";

static int openfile(void)
{
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd < 0) {
        t_error("open file failed\n");
        return -1;
    }
    return fd;
}

static void clearfile(void)
{
    if (access(path, F_OK) == 0) {
        remove(path);
    }
}

/**
 * @tc.name      : posix_madvise_0100
 * @tc.desc      : Verify that the return value result of creating a reasonable data page is as expected
 * @tc.level     : Level 1
 */
void posix_madvise_0100(void)
{
    size_t length = 4096;

    int fd = openfile();
    EXPECT_NE("posix_madvise_0100", fd, ERREXPECT);

    void *ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_PTRNE("posix_madvise_0100", ptr, MAP_FAILED);
    if (ptr != MAP_FAILED) {
        EXPECT_EQ("posix_madvise_0100", posix_madvise(ptr, length, MADV_NORMAL), CMPFLAG);
    }
    munmap(ptr, length);
    close(fd);
    clearfile();
}

/**
 * @tc.name      : posix_madvise_0200
 * @tc.desc      : Verify that the return value result of creating a super long data page is as expected
 * @tc.level     : Level 2
 */
void posix_madvise_0200(void)
{
    size_t length = 4096;
    int fd = openfile();

    EXPECT_NE("posix_madvise_0200", fd, ERREXPECT);
    void *ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    EXPECT_PTRNE("posix_madvise_0200", ptr, MAP_FAILED);
    if (ptr != MAP_FAILED) {
        size_t errorlen = -1;
        int ret = posix_madvise(ptr, errorlen, MADV_NORMAL);
        EXPECT_GT("posix_madvise_0200", ret, CMPFLAG);
    }
    munmap(ptr, length);
    close(fd);
    clearfile();
}

/**
 * @tc.name      : posix_madvise_0300
 * @tc.desc      : When the advice parameter is passed to MADV_DONTNEED
 *                 the function exits directly and returns 0
 * @tc.level     : Level 2
 */
void posix_madvise_0300(void)
{
    void *ptr = NULL;
    size_t len = 1024;
    int ret = posix_madvise(ptr, len, MADV_DONTNEED);
    EXPECT_EQ("posix_madvise_0300", ret, CMPFLAG);
}

int main(void)
{
    posix_madvise_0100();
    posix_madvise_0200();
    posix_madvise_0300();

    return t_status;
}

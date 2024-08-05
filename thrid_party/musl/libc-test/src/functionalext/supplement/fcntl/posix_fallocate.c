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
#include <sys/stat.h>
#include "functionalext.h"

#define TEST_LEN 10

/**
 * @tc.name      : posix_fallocate_0100
 * @tc.desc      : Allocate a specified amount of storage space for a file
 * @tc.level     : Level 0
 */
void posix_fallocate_0100(void)
{
    struct stat sb;
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("posix_fallocate_0100", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    EXPECT_TRUE("posix_fallocate_0100", fd != -1);
    if (fd == -1) {
        fclose(fp);
        return;
    }
    int ret = posix_fallocate(fd, 0, TEST_LEN);
    EXPECT_EQ("posix_fallocate_0100", ret, 0);
    ret = fstat(fd, &sb);
    EXPECT_EQ("posix_fallocate_0100", ret, 0);
    EXPECT_EQ("posix_fallocate_0100", sb.st_size, TEST_LEN);
    ret = fclose(fp);
    EXPECT_EQ("posix_fallocate_0100", ret, 0);
}

/**
 * @tc.name      : posix_fallocate_0200
 * @tc.desc      : Allocate -1 byte of storage for the file
 * @tc.level     : Level 2
 */
void posix_fallocate_0200(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("posix_fallocate_0200", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    EXPECT_TRUE("posix_fallocate_0200", fd != -1);
    if (fd == -1) {
        fclose(fp);
        return;
    }
    int ret = posix_fallocate(fd, 0, -1);
    EXPECT_EQ("posix_fallocate_0200", ret, EINVAL);
    ret = fclose(fp);
    EXPECT_EQ("posix_fallocate_0200", ret, 0);
}

/**
 * @tc.name      : posix_fallocate_0300
 * @tc.desc      : At file offset -1 byte, allocate 10 bytes of storage for the file
 * @tc.level     : Level 2
 */
void posix_fallocate_0300(void)
{
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("posix_fallocate_0300", fp, NULL);
        return;
    }

    int fd = fileno(fp);
    EXPECT_TRUE("posix_fallocate_0300", fd != -1);
    if (fd == -1) {
        fclose(fp);
        return;
    }
    int ret = posix_fallocate(fd, -1, TEST_LEN);
    EXPECT_EQ("posix_fallocate_0300", ret, EINVAL);
    ret = fclose(fp);
    EXPECT_EQ("posix_fallocate_0300", ret, 0);
}

/**
 * @tc.name      : posix_fallocate_0400
 * @tc.desc      : Bad file handle provided, allocating 10 bytes of storage for file
 * @tc.level     : Level 2
 */
void posix_fallocate_0400(void)
{
    int ret = posix_fallocate(-1, 0, TEST_LEN);
    EXPECT_EQ("posix_fallocate_0400", ret, EBADF);
}

int main(void)
{
    posix_fallocate_0100();
    posix_fallocate_0200();
    posix_fallocate_0300();
    posix_fallocate_0400();
    return t_status;
}
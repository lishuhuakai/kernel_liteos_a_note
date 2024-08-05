/*
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
#include <sys/file.h>
#include "functionalext.h"

/**
 * @tc.name      : flock_0100
 * @tc.desc      : Each parameter is valid, the op parameter is LOCK_SH, which can lock the file
 * @tc.level     : Level 0
 */
void flock_0100(void)
{
    int result = -1;
    int fd = open("/data/test.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_NE("flock_0100", fd, -1);
    lseek(fd, 2, SEEK_SET);

    result = flock(fd, LOCK_SH);
    EXPECT_EQ("flock_0100", result, 0);
    close(fd);
    remove("/data/test.txt");
}

/**
 * @tc.name      : flock_0200
 * @tc.desc      : Each parameter is valid, the op parameter is LOCK_SH|LOCK_NB, which can lock the file
 * @tc.level     : Level 1
 */
void flock_0200(void)
{
    int result = -1;
    int fd = open("/data/test.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_NE("flock_0200", fd, -1);
    lseek(fd, 2, SEEK_SET);

    result = flock(fd, LOCK_SH | LOCK_NB);
    EXPECT_EQ("flock_0200", result, 0);
    close(fd);
    remove("/data/test.txt");
}

/**
 * @tc.name      : flock_0300
 * @tc.desc      : Each parameter is valid, the op parameter is LOCK_EX, which can lock the file
 * @tc.level     : Level 0
 */
void flock_0300(void)
{
    int result = -1;
    int fd = open("/data/test.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_NE("flock_0300", fd, -1);
    lseek(fd, 2, SEEK_SET);

    result = flock(fd, LOCK_EX);
    EXPECT_EQ("flock_0300", result, 0);
    close(fd);
    remove("/data/test.txt");
}

/**
 * @tc.name      : flock_0400
 * @tc.desc      : Each parameter is valid, the op parameter is LOCK_EX|LOCK_NB, which can lock the file
 * @tc.level     : Level 1
 */
void flock_0400(void)
{
    int result = -1;
    int fd = open("/data/test.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_NE("flock_0400", fd, -1);
    lseek(fd, 2, SEEK_SET);

    result = flock(fd, LOCK_EX | LOCK_NB);
    EXPECT_EQ("flock_0400", result, 0);
    close(fd);
    remove("/data/test.txt");
}

/**
 * @tc.name      : flock_0500
 * @tc.desc      : Each parameter is valid, the op parameter is LOCK_UN, which can release the file lock state
 * @tc.level     : Level 0
 */
void flock_0500(void)
{
    int result = -1;
    int fd = open("/data/test.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_NE("flock_0500", fd, -1);
    lseek(fd, 2, SEEK_SET);

    result = flock(fd, LOCK_UN);
    EXPECT_EQ("flock_0500", result, 0);
    close(fd);
    remove("/data/test.txt");
}

/**
 * @tc.name      : flock_0600
 * @tc.desc      : The fd parameter is invalid, the file cannot be locked
 * @tc.level     : Level 2
 */
void flock_0600(void)
{
    int result = flock(-1, LOCK_EX);
    EXPECT_EQ("flock_0600", result, -1);
}

/**
 * @tc.name      : flock_0700
 * @tc.desc      : The op parameter is invalid, the file cannot be locked
 * @tc.level     : Level 2
 */
void flock_0700()
{
    int result = 0;
    int fd = open("/data/test.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_NE("flock_0700", fd, -1);
    lseek(fd, 2, SEEK_SET);

    result = flock(fd, 0);
    EXPECT_EQ("flock_0700", result, -1);
    close(fd);
    remove("/data/test.txt");
}

int main(int argc, char *argv[])
{
    flock_0100();
    flock_0200();
    flock_0300();
    flock_0400();
    flock_0500();
    flock_0600();
    flock_0700();
    return t_status;
}
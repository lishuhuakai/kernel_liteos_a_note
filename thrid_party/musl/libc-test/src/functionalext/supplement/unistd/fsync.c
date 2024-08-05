/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <unistd.h>
#include "functionalext.h"

const char *path = "/data/test.txt";

/**
 * @tc.name      : fsync_0100
 * @tc.desc      : Tests of fsync on file descriptors in different modes
 * @tc.level     : Level 0
 */
void fsync_0100(void)
{
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fsync_0100", fd >= 0);

    int result = fsync(fd);
    EXPECT_EQ("fsync_0100", result, 0);
    close(fd);

    fd = open(path, O_RDONLY);
    result = fsync(fd);
    EXPECT_EQ("fsync_0100", result, 0);
    close(fd);

    remove(path);
}

/**
 * @tc.name      : fsync_0200
 * @tc.desc      : fd is a directory
 * @tc.level     : Level 1
 */
void fsync_0200(void)
{
    int fd = open("/data", O_RDONLY);
    EXPECT_NE("fsync_0200", fd, -1);

    int result = fsync(fd);
    EXPECT_EQ("fsync_0200", result, 0);
    close(fd);
}

/**
 * @tc.name      : fsync_0300
 * @tc.desc      : Can't sync an invalid fd
 * @tc.level     : Level 2
 */
void fsync_0300(void)
{
    errno = 0;
    int result = fsync(-1);
    EXPECT_EQ("fsync_0300", result, -1);
    EXPECT_EQ("fsync_0300", errno, EBADF);
}

int main(int argc, char *argv[])
{
    fsync_0100();
    fsync_0200();
    fsync_0300();

    return t_status;
}
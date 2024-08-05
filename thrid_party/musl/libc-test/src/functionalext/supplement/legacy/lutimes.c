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
#include <sys/stat.h>
#include <sys/time.h>
#include "functionalext.h"

#define TEST_FIFO_MODE 777
#define TEST_SIZE 2

extern int __lutimes_time64(const char *, const struct timeval [2]);

/**
 * @tc.name      : lutimes_0100
 * @tc.desc      : Verify inet_lnaofof process success. Lfutimes can modify the timestamp of the file.
 * @tc.level     : Level 0
 */
void lutimes_0100(void)
{
    int ret;
    struct stat s;
    static struct timeval tv[TEST_SIZE] = {{0L, 0L}, {0L, 0L}};
    tv[0].tv_sec = s.st_atime;
    tv[0].tv_usec = 0;
    tv[1].tv_sec = s.st_mtime;
    tv[1].tv_usec = 0;
    int fd = -1;
    fd = creat("./lutimes_0100.txt", TEST_FIFO_MODE);
    if (fd < 0) {
        EXPECT_MT("lutimes_0100", fd, 0);
        return;
    } else {
        close(fd);
    }
    ret = lutimes("./lutimes_0100.txt", tv);
    EXPECT_EQ("lutimes_0100", ret, 0);
    ret = remove("lutimes_0100.txt");
    EXPECT_EQ("lutimes_0100", ret, 0);
}

/**
 * @tc.name      : lutimes_0200
 * @tc.desc      : Verify inet_lnaofof process fail. Because the fd argument is invalid
 *                 and the lutimes function cannot modify the timestamp of the file.
 * @tc.level     : Level 2
 */
void lutimes_0200(void)
{
    int ret = -1;
    static struct timeval tv[TEST_SIZE] = {{0L, 0L}, {0L, 0L}};
    ret = lutimes("./lutimes_0200.txt", tv);
    EXPECT_EQ("lutimes_0200", ret, -1);
}

/**
 * @tc.name      : lutimes_time64_0100
 * @tc.desc      : Verify inet_lnaofof process success. Lfutimes can modify the timestamp of the file.
 * @tc.level     : Level 0
 */
void lutimes_time64_0100(void)
{
    int ret;
    struct stat s;
    static struct timeval tv[TEST_SIZE] = {{0L, 0L}, {0L, 0L}};
    tv[0].tv_sec = s.st_atime;
    tv[0].tv_usec = 0;
    tv[1].tv_sec = s.st_mtime;
    tv[1].tv_usec = 0;
    int fd = -1;
    fd = creat("./lutimes_time64_0100.txt", TEST_FIFO_MODE);
    if (fd < 0) {
        EXPECT_MT("lutimes_time64_0100", fd, 0);
        return;
    } else {
        close(fd);
    }
    ret = __lutimes_time64("./lutimes_time64_0100.txt", tv);
    EXPECT_EQ("lutimes_time64_0100", ret, 0);
    ret = remove("lutimes_time64_0100.txt");
    EXPECT_EQ("lutimes_time64_0100", ret, 0);
}

int main(void)
{
    lutimes_0100();
    lutimes_0200();
    lutimes_time64_0100();
    return t_status;
}
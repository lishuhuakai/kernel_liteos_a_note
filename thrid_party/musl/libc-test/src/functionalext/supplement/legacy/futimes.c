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
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include "functionalext.h"

const int FAIL = -1;
const int SUCCESS = 0;

extern int __futimes_time64(int, const struct timeval [2]);

/**
 * @tc.name      : futimes_0100
 * @tc.desc      : All parameters are valid, TV is not 0, and futimes can modify the timestamp of the file.
 * @tc.level     : Level 0
 */
void futimes_0100(void)
{
    int ret = -1;
    struct stat s;
    static struct timeval tv[2] = {{0L, 0L}, {0L, 0L}};
    tv[0].tv_sec = s.st_atime;
    tv[0].tv_usec = 0;
    tv[1].tv_sec = s.st_mtime;
    tv[1].tv_usec = 0;
    int fd = open("futimes.txt", O_RDWR | O_CREAT, 777);
    ret = futimes(fd, tv);
    EXPECT_EQ("futimes_0100", ret, SUCCESS);
    remove("futimes.txt");
}

/**
 * @tc.name      : futimes_0200
 * @tc.desc      : All parameters are valid, TV is 0, and futimes can modify the timestamp of the file.
 * @tc.level     : Level 1
 */
void futimes_0200(void)
{
    int ret = -1;
    int fd = open("futimes.txt", O_RDWR | O_CREAT, 777);
    ret = futimes(fd, 0);
    EXPECT_EQ("futimes_0200", ret, SUCCESS);
    remove("futimes.txt");
}

/**
 * @tc.name      : futimes_0300
 * @tc.desc      : The fd argument is invalid and the futimes function cannot modify the timestamp of the file.
 * @tc.level     : Level 2
 */
void futimes_0300(void)
{
    int ret = -1;
    static struct timeval tv[2] = {{0L, 0L}, {0L, 0L}};
    int fd = open("futimes.txt", O_RDWR);
    ret = futimes(fd, tv);
    EXPECT_EQ("futimes_0300", ret, FAIL);
}

/**
 * @tc.name      : futimes_time64_0100
 * @tc.desc      : All parameters are valid, TV is not 0, and futimes can modify the timestamp of the file.
 * @tc.level     : Level 0
 */
void futimes_time64_0100(void)
{
    int ret = -1;
    struct stat s;
    static struct timeval tv[2] = {{0L, 0L}, {0L, 0L}};
    tv[0].tv_sec = s.st_atime;
    tv[0].tv_usec = 0;
    tv[1].tv_sec = s.st_mtime;
    tv[1].tv_usec = 0;
    int fd = open("futimes.txt", O_RDWR | O_CREAT, 777);
    ret = __futimes_time64(fd, tv);
    EXPECT_EQ("futimes_time64_0100", ret, SUCCESS);
    remove("futimes.txt");
}

int main(int argc, char *argv[])
{
    futimes_0100();
    futimes_0200();
    futimes_0300();
    futimes_time64_0100();
    return t_status;
}
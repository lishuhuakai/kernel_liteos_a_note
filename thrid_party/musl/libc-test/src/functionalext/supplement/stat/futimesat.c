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
#include "test.h"

const char *path = "/data";

extern int __futimesat_time64(int, const char *, const struct timeval [2]);

/**
 * @tc.name      : futimesat_0100
 * @tc.desc      : Change timestamps of a file relative to a directory file descriptor
 * @tc.level     : Level 0
 */
void futimesat_0100(void)
{
    int dir_fd = open(path, O_RDONLY | O_DIRECTORY);
    if (dir_fd < 0) {
        t_error("%s open failed\n", __func__);
    }

    int fd = openat(dir_fd, "test.txt", O_CREAT | O_RDWR | O_EXCL, 0666);
    if (fd < 0) {
        t_error("%s openat failed\n", __func__);
    }

    write(fd, "helloworld", 5);

    struct stat st1;
    if (fstat(fd, &st1) != 0) {
        t_error("%s fstat failed\n", __func__);
    }
    close(fd);

    struct timeval tv[2];
    tv[0].tv_sec = st1.st_atime + 1;
    tv[0].tv_usec = 0;
    tv[1].tv_sec = st1.st_mtime + 1;
    tv[1].tv_usec = 0;

    int result = futimesat(dir_fd, "test.txt", tv);
    if (result != 0) {
        t_error("%s futimesat failed\n", __func__);
    }

    struct stat st2;
    if (fstatat(dir_fd, "test.txt", &st2, 0) != 0) {
        t_error("%s fstatat failed\n", __func__);
    }

    if (st2.st_mtime != tv[1].tv_sec) {
        t_error("%s stat shows different mtime\n", __func__);
    }

    if (unlinkat(dir_fd, "test.txt", 0) != 0) {
        t_error("%s unlinkat failed\n", __func__);
    }

    close(dir_fd);
}

/**
 * @tc.name      : futimesat_0200
 * @tc.desc      : Test the return value of the function when timeval is NULL
 * @tc.level     : Level 1
 */
void futimesat_0200(void)
{
    int dir_fd = open(path, O_RDONLY | O_DIRECTORY);
    if (dir_fd < 0) {
        t_error("%s open failed\n", __func__);
    }

    int fd = openat(dir_fd, "test.txt", O_CREAT | O_RDWR | O_EXCL, 0666);
    if (fd < 0) {
        t_error("%s openat failed\n", __func__);
    }

    int result = futimesat(dir_fd, "test.txt", NULL);
    if (result != 0) {
        t_error("%s futimesat failed\n", __func__);
    }

    if (unlinkat(dir_fd, "test.txt", 0) != 0) {
        t_error("%s unlinkat failed\n", __func__);
    }

    close(dir_fd);
}

/**
 * @tc.name      : futimesat_0300
 * @tc.desc      : Test the return value of the function when dirfd is invalid
 * @tc.level     : Level 2
 */
void futimesat_0300(void)
{
    int result = futimesat(-1, "test.txt", NULL);
    if (result != -1) {
        t_error("%s futimesat should be failed\n", __func__);
    }
}

/**
 * @tc.name      : futimesat_0400
 * @tc.desc      : Test the return value of the function when pathname is invalid
 * @tc.level     : Level 2
 */
void futimesat_0400(void)
{
    int result = futimesat(AT_FDCWD, NULL, NULL);
    if (result != -1) {
        t_error("%s futimesat should be failed\n", __func__);
    }
}

/**
 * @tc.name      : futimesat_time64_0200
 * @tc.desc      : Test the return value of the function when timeval is NULL
 * @tc.level     : Level 1
 */
void futimesat_time64_0200(void)
{
    int dir_fd = open(path, O_RDONLY | O_DIRECTORY);
    if (dir_fd < 0) {
        t_error("%s open failed\n", __func__);
    }

    int fd = openat(dir_fd, "test.txt", O_CREAT | O_RDWR | O_EXCL, 0666);
    if (fd < 0) {
        t_error("%s openat failed\n", __func__);
    }

    int result = __futimesat_time64(dir_fd, "test.txt", NULL);
    if (result != 0) {
        t_error("%s __futimesat_time64 failed\n", __func__);
    }

    if (unlinkat(dir_fd, "test.txt", 0) != 0) {
        t_error("%s unlinkat failed\n", __func__);
    }

    close(dir_fd);
}

int main(int argc, char *argv[])
{
    futimesat_0100();
    futimesat_0200();
    futimesat_0300();
    futimesat_0400();
    futimesat_time64_0200();
    return t_status;
}
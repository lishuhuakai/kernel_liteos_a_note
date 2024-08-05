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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include "filepath_util.h"

const long sec = 123840;

extern int __utimensat_time64(int, const char *, const struct timespec [2], int);

/**
 * @tc.name      : utimensat_0100
 * @tc.desc      : change file timestamps with nanosecond precision
 * @tc.level     : Level 0
 */
void utimensat_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    struct timespec times[] = {{.tv_sec = 0}, {.tv_sec = sec}};

    int result = utimensat(fd, path, times, 0);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    close(fd);

    struct stat statbuf;
    result = stat(path, &statbuf);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (statbuf.st_mtim.tv_sec != sec) {
        t_error("%s failed: statbuf.st_mtim.tv_sec = %ld\n", __func__, statbuf.st_mtim.tv_sec);
    }

    remove(path);
}

/**
 * @tc.name      : utimensat_0200
 * @tc.desc      : change file timestamps with invalid parameters
 * @tc.level     : Level 2
 */
void utimensat_0200(void)
{
    errno = 0;
    int result = utimensat(-1, NULL, NULL, 0);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != EBADF) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : utimensat_time64_0100
 * @tc.desc      : change file timestamps with nanosecond precision
 * @tc.level     : Level 0
 */
void utimensat_time64_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    struct timespec times[] = {{.tv_sec = 0}, {.tv_sec = sec}};

    int result = __utimensat_time64(fd, path, times, 0);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    close(fd);

    struct stat statbuf;
    result = stat(path, &statbuf);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (statbuf.st_mtim.tv_sec != sec) {
        t_error("%s failed: statbuf.st_mtim.tv_sec = %ld\n", __func__, statbuf.st_mtim.tv_sec);
    }

    remove(path);
}

int main(int argc, char *argv[])
{
    utimensat_0100();
    utimensat_0200();
    utimensat_time64_0100();

    return t_status;
}

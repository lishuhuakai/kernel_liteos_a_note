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
#include <unistd.h>

#include "filepath_util.h"

/**
 * @tc.name      : sync_file_range_0100
 * @tc.desc      : Check simple successful case
 * @tc.level     : Level 0
 */
void sync_file_range_0100(void)
{
    errno = 0;
    int result, fd;
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_SYNC_TXT, path);

    fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd == -1) {
        t_error("%s open failed\n", __func__);
        return;
    }

    result = sync_file_range(fd, 0, 1024, 0);
    if (result == -1) {
        t_error("%s sync_file_range failed, result is %d\n", __func__, result);
    }
    close(fd);
    remove(path);
}

/**
 * @tc.name      : sync_file_range_0200
 * @tc.desc      : Check for invalid file descriptor
 * @tc.level     : Level 2
 */
void sync_file_range_0200(void)
{
    errno = 0;
    int result;

    result = sync_file_range(-1, 0, 0, 0);
    if (result != -1) {
        t_error("%s sync_file_range should failed, result is %d\n", __func__, result);
    }
    if (errno != EBADF) {
        t_error("%s sync_file_range did not set errno to EBADF\n", __func__);
    }
}

/**
 * @tc.name      : sync_file_range_0300
 * @tc.desc      : Check for invalid flags
 * @tc.level     : Level 2
 */
void sync_file_range_0300(void)
{
    errno = 0;
    int result, fd;

    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_SYNC_TXT, path);
    fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd == -1) {
        t_error("%s open failed\n", __func__);
        return;
    }

    result = sync_file_range(fd, 0, 0, -1);
    if (result != -1) {
        t_error("%s sync_file_range did not failed with invalid flags, result is %d\n", __func__, result);
    }
    if (errno != EINVAL) {
        t_error("%s sync_file_range did not set errno to EINVAL\n", __func__);
    }
    close(fd);
    remove(path);
}

/**
 * @tc.name      : sync_file_range_0400
 * @tc.desc      : Check for negative offset
 * @tc.level     : Level 2
 */
void sync_file_range_0400(void)
{
    errno = 0;
    int result, fd;

    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_SYNC_TXT, path);
    fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd == -1) {
        t_error("%s open failed\n", __func__);
        return;
    }

    result = sync_file_range(fd, -1, -1, 0);
    if (result != -1) {
        t_error("%s sync_file_range did not failed with invalid offset, result is %d\n", __func__, result);
    }
    if (errno != EINVAL) {
        t_error("%s sync_file_range did not set errno to EINVAL\n", __func__);
    }
    close(fd);
    remove(path);
}

int main(int argc, char *argv[])
{
    sync_file_range_0100();
    sync_file_range_0200();
    sync_file_range_0300();
    sync_file_range_0400();
    return t_status;
}

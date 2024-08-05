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
#include <string.h>
#include "filepath_util.h"

const char *str = "Hello";
/**
 * @tc.name      : copy_file_range_0100
 * @tc.desc      : Copy a range of data from one file to another
 * @tc.level     : Level 0
 */
void copy_file_range_0100(void)
{
    int fd_in, fd_out, wlen, result;
    char buffer[BUFSIZ];

    char path_in[PATH_MAX] = {0};
    char path_out[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_IN_TXT, path_in);
    FILE_ABSOLUTE_PATH(STR_FILE_OUT_TXT, path_out);

    fd_in = open(path_in, O_RDWR | O_CREAT, TEST_MODE);
    if (fd_in == -1) {
        t_error("%s open path_in failed\n", __func__);
        return;
    }

    wlen = write(fd_in, str, strlen(str));
    if (wlen != strlen(str)) {
        t_error("%s write failed\n", __func__);
        return;
    }

    close(fd_in);
    fd_in = open(path_in, O_RDONLY);

    fd_out = open(path_out, O_RDWR | O_CREAT, TEST_MODE);
    if (fd_out == -1) {
        t_error("%s open path_out failed\n", __func__);
        return;
    }

    do {
        result = copy_file_range(fd_in, NULL, fd_out, NULL, wlen, 0);
        if (result == -1) {
            t_error("%s copy_file_range failed\n", __func__);
            return;
        }
        wlen -= result;
    } while (wlen > 0 && result > 0);

    close(fd_out);
    fd_out = open(path_out, O_RDONLY);
    memset(buffer, 0, sizeof(buffer));
    result = read(fd_out, buffer, sizeof(buffer));
    if (result == -1) {
        t_error("%s read failed\n", __func__);
    }
    if (strcmp(str, buffer)) {
        t_error("%s buffer is %s, not %s", __func__, buffer, str);
    }

    close(fd_in);
    close(fd_out);
    remove(path_in);
    remove(path_out);
}

/**
 * @tc.name      : copy_file_range_0200
 * @tc.desc      : One or more file descriptors are not valid
 * @tc.level     : Level 2
 */
void copy_file_range_0200(void)
{
    errno = 0;
    int result = copy_file_range(-1, NULL, -1, NULL, 0, 0);
    if (result != -1) {
        t_error("%s copy_file_range should failed, result is %d\n", __func__, result);
    }
    if (errno != EBADF) {
        t_error("%s errno is %d, not EBADF", __func__, errno);
    }
}

/**
 * @tc.name      : copy_file_range_0300
 * @tc.desc      : File permissions are restricted
 * @tc.level     : Level 2
 */
void copy_file_range_0300(void)
{
    int fd_in, fd_out, wlen, result;
    char buffer[BUFSIZ];

    char path_in[PATH_MAX] = {0};
    char path_out[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_IN_TXT, path_in);
    FILE_ABSOLUTE_PATH(STR_FILE_OUT_TXT, path_out);

    fd_in = open(path_in, O_WRONLY | O_CREAT, TEST_MODE);
    if (fd_in == -1) {
        t_error("%s open path_in failed\n", __func__);
        return;
    }

    wlen = write(fd_in, str, strlen(str));
    if (wlen != strlen(str)) {
        t_error("%s write failed\n", __func__);
        return;
    }

    close(fd_in);
    fd_in = open(path_in, O_WRONLY);

    fd_out = open(path_out, O_RDONLY | O_CREAT, TEST_MODE);
    if (fd_out == -1) {
        t_error("%s open path_out failed\n", __func__);
        return;
    }

    errno = 0;
    result = copy_file_range(fd_in, NULL, fd_out, NULL, wlen, 0);
    if (result != -1) {
        t_error("%s copy_file_range should failded\n", __func__);
    }
    if (errno != EBADF) {
        t_error("%s errno is %d, not EBADF\n", __func__, errno);
    }

    close(fd_in);
    close(fd_out);
    remove(path_in);
    remove(path_out);
}

int main(int argc, char *argv[])
{
    copy_file_range_0100();
    copy_file_range_0200();
    copy_file_range_0300();
    return t_status;
}
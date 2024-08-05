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

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : symlinkat_0100
 * @tc.desc      : Create a symbolic link to the specified file
 * @tc.level     : Level 0
 */
void symlinkat_0100(void)
{
    char buffer[BUFSIZ];
    char *path_name = "/data";
    char *file_name = "/data/symlinkat.txt";
    char *symlink_name = "mysymlinkat";
    ssize_t sz;
    DIR *dp;
    int fdd, fdf;

    fdf = creat(file_name, O_RDWR | O_CREAT);
    if (fdf == -1) {
        t_error("%s creat failed", __func__);
    }

    dp = opendir(path_name);
    if (dp == NULL) {
        t_error("%s opendir failed", __func__);
        return;
    }

    fdd = dirfd(dp);
    if (fdd < 0) {
        t_error("%s dirfd failed", __func__);
        return;
    }

    if (symlinkat(file_name, fdd, symlink_name) < 0) {
        t_error("%s symlinkat failed", __func__);
        return;
    }

    memset(buffer, 0, sizeof(buffer));
    sz = readlinkat(fdd, symlink_name, buffer, sizeof(buffer));
    if (sz < 0) {
        t_error("%s readlinkat failed", __func__);
        return;
    }

    if (strcmp(buffer, file_name)) {
        t_error("%s buffer is %s, not %s", __func__, buffer, file_name);
    }

    unlinkat(fdd, symlink_name, 0);
    unlink(file_name);
}

/**
 * @tc.name      : symlinkat_0200
 * @tc.desc      : The file pointed to by the parameter existing does not exist
 * @tc.level     : Level 1
 */
void symlinkat_0200(void)
{
    char buffer[BUFSIZ];
    char *path_name = "/data";
    char *file_name = "/data/symlinkat.txt";
    char *symlink_name = "mysymlinkat";
    ssize_t sz;
    DIR *dp;
    int fd;

    dp = opendir(path_name);
    if (dp == NULL) {
        t_error("%s opendir failed", __func__);
        return;
    }

    fd = dirfd(dp);
    if (fd < 0) {
        t_error("%s dirfd failed", __func__);
        return;
    }

    if (symlinkat(file_name, fd, symlink_name) < 0) {
        t_error("%s symlinkat failed", __func__);
        return;
    }

    unlinkat(fd, symlink_name, 0);
}

int main(int argc, char *argv[])
{
    symlinkat_0100();
    symlinkat_0200();
    return t_status;
}
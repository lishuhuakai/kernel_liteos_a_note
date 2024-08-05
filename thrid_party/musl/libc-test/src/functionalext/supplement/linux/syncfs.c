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
 * @tc.name      : syncfs_0100
 * @tc.desc      : Commit filesystem caches to disk
 * @tc.level     : Level 0
 */
void syncfs_0100(void)
{
    errno = 0;
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("syncfs.txt", path);
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd == -1) {
        t_error("%s open failed\n", __func__);
        return;
    }

    int result = syncfs(fd);
    if (result == -1) {
        t_error("%s syncfs failed, result is %d\n", __func__, result);
    }
    if (errno == EBADF) {
        t_error("%s errno is %d\n", __func__, errno);
    }
    close(fd);
    remove(path);
}

/**
 * @tc.name      : syncfs_0200
 * @tc.desc      : Can't sync an invalid fd
 * @tc.level     : Level 2
 */
void syncfs_0200(void)
{
    errno = 0;
    int result = syncfs(-1);
    if (result != -1) {
        t_error("%s syncfs should failed, resuly is %d\n", __func__, result);
    }
    if (errno != EBADF) {
        t_error("%s errno is %d\n", errno);
    }
}

int main(int argc, char *argv[])
{
    syncfs_0100();
    syncfs_0200();
    return t_status;
}

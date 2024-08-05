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

#include "filepath_util.h"

/**
 * @tc.name      : unlinkat_0100
 * @tc.desc      : delete a name and possibly the file it refers to
 * @tc.level     : Level 0
 */
void unlinkat_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    int fd = open(path, O_CREAT, 0664);

    int result = unlinkat(fd, path, 0);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    close(fd);
}

/**
 * @tc.name      : unlinkat_0200
 * @tc.desc      : delete a name and possibly the file it refers to with AT_REMOVEDIR flag
 * @tc.level     : Level 2
 */
void unlinkat_0200(void)
{
    errno = 0;
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    int result = unlinkat(AT_FDCWD, path, AT_REMOVEDIR);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != ENOENT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    unlinkat_0100();
    unlinkat_0200();

    return t_status;
}

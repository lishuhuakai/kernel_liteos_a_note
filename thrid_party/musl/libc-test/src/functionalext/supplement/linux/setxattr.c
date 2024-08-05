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
#include <sys/xattr.h>

#include "filepath_util.h"

const char *name = "user.foo";
const char *value = "bar";

/**
 * @tc.name      : setxattr_0100
 * @tc.desc      : set an extended attribute value
 * @tc.level     : Level 0
 */
void setxattr_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd < 0) {
        t_error("%s failed: fd = %d\n", __func__, fd);
    }

    char str[] = "bar";
    write(fd, str, sizeof(str));
    close(fd);

    errno = 0;
    int result = setxattr(path, name, value, strlen(value), XATTR_CREATE);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    char buf[BUFSIZ] = {0};

    errno = 0;
    result = getxattr(path, name, buf, sizeof(buf));
    if (result < 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    if (strcmp(buf, str)) {
        t_error("%s failed: buf = %s\n", __func__, buf);
    }

    remove(path);
}

/**
 * @tc.name      : setxattr_0200
 * @tc.desc      : set an extended attribute value with invalid parameters
 * @tc.level     : Level 2
 */
void setxattr_0200(void)
{
    errno = 0;
    int result = setxattr(NULL, NULL, NULL, -1, -1);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != EFAULT) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    setxattr_0100();
    setxattr_0200();

    return t_status;
}

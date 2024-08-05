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
#include <sys/uio.h>

#include "filepath_util.h"

/**
 * @tc.name      : readv_0100
 * @tc.desc      : read data into multiple buffers
 * @tc.level     : Level 0
 */
void readv_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, path);
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);

    char buf1[] = "hello";
    char buf2[] = "world";
    struct iovec ios[] = {{buf1, strlen(buf1)}, {buf2, strlen(buf2)}};

    ssize_t result = writev(fd, ios, sizeof(ios) / sizeof(struct iovec));
    if (result != strlen(buf1) + strlen(buf2)) {
        t_error("%s failed: result = %ld\n", __func__, result);
    }

    lseek(fd, 0, SEEK_SET);

    memset(buf1, 0, sizeof(buf1));
    memset(buf2, 0, sizeof(buf2));

    result = readv(fd, ios, sizeof(ios) / sizeof(struct iovec));
    if (result != strlen(buf1) + strlen(buf2)) {
        t_error("%s failed: result = %ld\n", __func__, result);
    }

    if (strcmp(buf1, "hello")) {
        t_error("%s failed: buf1 = %s\n", __func__, buf1);
    }

    if (strcmp(buf2, "world")) {
        t_error("%s failed: buf2 = %s\n", __func__, buf2);
    }

    remove(path);
}

/**
 * @tc.name      : readv_0200
 * @tc.desc      : read data into multiple buffers with invalid parameters
 * @tc.level     : Level 2
 */
void readv_0200(void)
{
    errno = 0;
    ssize_t result = readv(-1, NULL, -1);
    if (result == 0) {
        t_error("%s failed: result = %ld\n", __func__, result);
    }

    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    readv_0100();
    readv_0200();

    return t_status;
}

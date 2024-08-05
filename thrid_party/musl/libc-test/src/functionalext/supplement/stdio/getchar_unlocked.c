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

#include <fcntl.h>
#include <stdio.h>
#include "test.h"

const char *path = "/data/readtest.txt";

/**
 * @tc.name      : getchar_unlocked_0100
 * @tc.desc      : Verify that a character can be read from standard input.
 * @tc.level     : Level 0
 */
void getchar_unlocked_0100(void)
{
    char str[] = "r";
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    if (fd < 0) {
        t_error("%s open failed\n", __func__);
    }

    ssize_t ret = write(fd, str, sizeof(str));
    if (ret < 0) {
        t_error("%s write failed\n", __func__);
    }
    FILE *fp = freopen(path, "r", stdin);
    if (!fp) {
        t_error("%s freopen failed\n", __func__);
    }

    char ch = getchar_unlocked();
    if (ch != 'r') {
        t_error("%s getchar_unlocked failed\n");
    }

    fclose(fp);
    close(fd);
    remove(path);
}

int main(int argc, char *argv[])
{
    getchar_unlocked_0100();
    return t_status;
}
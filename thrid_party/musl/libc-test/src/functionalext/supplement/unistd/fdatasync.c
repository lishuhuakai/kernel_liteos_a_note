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
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

const char *path = "/data/readtest.txt";

/**
 * @tc.name      : fdatasync_0100
 * @tc.desc      : Verify flush kernel's cache (parameter valid)
 * @tc.level     : Level 0
 */
void fdatasync_0100()
{
    char str[] = "hello";
    char buffer[1024] = {"\0"};
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_NE("fdatasync_0100", fd, -1);

    write(fd, str, sizeof(str));
    lseek(fd, 0L, SEEK_SET);
    read(fd, buffer, 1024);

    int result = fdatasync(fd);
    EXPECT_EQ("fdatasync_0100", result, 0);
    EXPECT_STREQ("fdatasync_0100", buffer, "hello");

    close(fd);
    remove(path);
}

/**
 * @tc.name      : fdatasync_0200
 * @tc.desc      : Verify failed to flush kernel's cache (file close)
 * @tc.level     : Level 2
 */
void fdatasync_0200()
{
    char str[] = "hello\n";
    char buffer[1024] = {"\0"};
    int fd = open(path, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_NE("fdatasync_0100", fd, -1);
    close(fd);

    int result = fdatasync(fd);
    EXPECT_EQ("fdatasync_0200", result, -1);

    remove(path);
}

/**
 * @tc.name      : fdatasync_0300
 * @tc.desc      : Validation failed to flush kernel's cache (invalid parameter)
 * @tc.level     : Level 2
 */
void fdatasync_0300()
{
    int result = fdatasync(-1);
    EXPECT_EQ("fdatasync_0300", result, -1);
}

TEST_FUN G_Fun_Array[] = {
    fdatasync_0100,
    fdatasync_0200,
    fdatasync_0300,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
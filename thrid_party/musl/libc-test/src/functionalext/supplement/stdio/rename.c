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
#include "functionalext.h"

/**
 * @tc.name      : rename_0100
 * @tc.desc      : Each parameter is valid, and the file name can be changed.
 * @tc.level     : Level 0
 */
void rename_0100(void)
{
    const char *oldname = "oldfile.txt";
    const char *newname = "newfile.txt";
    int fd = open(oldname, O_RDONLY | O_CREAT, TEST_MODE);
    EXPECT_TRUE("rename_0100", fd != -1);
    close(fd);

    int ret = rename(oldname, newname);
    EXPECT_EQ("rename_0100", ret, 0);
    fd = open(oldname, O_RDONLY);
    EXPECT_EQ("rename_0100", fd, -1);
    fd = open(newname, O_RDONLY);
    EXPECT_NE("rename_0100", fd, -1);
    close(fd);
    remove(newname);
}

/**
 * @tc.name      : rename_0200
 * @tc.desc      : Parameter 1 is invalid, the file name cannot be changed.
 * @tc.level     : Level 2
 */
void rename_0200(void)
{
    const char *oldname = "oldfile.txt";
    const char *newname = "newfile.txt";
    int ret = rename(oldname, newname);
    EXPECT_EQ("rename_0200", ret, -1);
}

/**
 * @tc.name      : rename_0300
 * @tc.desc      : Newpath already exists, the file name can be changed.
 * @tc.level     : Level 1
 */
void rename_0300(void)
{
    const char *oldname = "oldfile.txt";
    const char *newname = "newfile.txt";
    int fd_old = open(oldname, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("rename_0300", fd_old != -1);

    char str_old[] = "old";
    int ret_old = write(fd_old, str_old, sizeof(str_old));
    EXPECT_TRUE("rename_0300", ret_old > 0);
    close(fd_old);

    int fd_new = open(newname, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("rename_0100", fd_new != -1);

    char str_new[] = "new";
    int ret_new = write(fd_old, str_new, sizeof(str_new));
    EXPECT_TRUE("rename_0300", ret_new > 0);
    close(fd_new);

    int result = rename(oldname, newname);
    EXPECT_EQ("rename_0300", result, 0);

    int fd = open(newname, O_RDONLY);
    EXPECT_TRUE("rename_0300", fd != -1);

    char buf[10];
    memset(buf, 0, sizeof(buf));
    int nread = read(fd, buf, sizeof(buf));
    EXPECT_TRUE("rename_0300", nread > 0);
    EXPECT_TRUE("rename_0300", strcmp(buf, "old") == 0);

    remove(newname);
}

int main(int argc, char *argv[])
{
    rename_0100();
    rename_0200();
    rename_0300();
    return t_status;
}
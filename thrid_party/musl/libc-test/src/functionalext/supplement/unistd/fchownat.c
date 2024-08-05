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
#include <sys/stat.h>
#include "functionalext.h"
#include "filepath_util.h"

/**
 * @tc.name      : fchownat_0100
 * @tc.desc      : Parameter fd is not equal to AT_FDCWD, the user and group that can change the owner of the file.
 * @tc.level     : Level 0
 */
void fchownat_0100(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FCHOWNAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fchownat_0100", fd >= 0);
    int ret = fchownat(fd, ptr, 0, 0, AT_SYMLINK_NOFOLLOW);
    EXPECT_EQ("fchownat_0100", ret, 0);
    struct stat buf;
    int st = stat(ptr, &buf);
    EXPECT_EQ("fchownat_0100", st, 0);
    EXPECT_EQ("fchownat_0100", buf.st_uid, 0);
    EXPECT_EQ("fchownat_0100", buf.st_gid, 0);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : fchownat_0200
 * @tc.desc      : Parameter fd equal to AT_FDCWD, the user and group that can change the owner of the file.
 * @tc.level     : Level 1
 */
void fchownat_0200(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FCHOWNAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fchownat_0200", fd >= 0);
    int ret = fchownat(AT_FDCWD, ptr, 0, 0, AT_SYMLINK_NOFOLLOW);
    EXPECT_EQ("fchownat_0200", ret, 0);
    struct stat buf;
    int st = stat(ptr, &buf);
    EXPECT_EQ("fchownat_0200", st, 0);
    EXPECT_EQ("fchownat_0200", buf.st_uid, 0);
    EXPECT_EQ("fchownat_0200", buf.st_gid, 0);
    close(fd);
    remove(ptr);
}

int main(int argc, char *argv[])
{
    fchownat_0100();
    fchownat_0200();
    return t_status;
}
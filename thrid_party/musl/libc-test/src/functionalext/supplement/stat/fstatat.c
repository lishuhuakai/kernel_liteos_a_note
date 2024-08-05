/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "functionalext.h"
#include "filepath_util.h"

const int SUCCESS = 0;

extern int __fstatat_time64(int, const char *__restrict, struct stat *__restrict, int);

/**
 * @tc.name      : fstatat_0100
 * @tc.desc      : The parameter fd is equal to AT_FDCWD, the flag is 0,
 *                 and the information of the file can be obtained.
 * @tc.level     : Level 0
 */
void fstatat_0100(void)
{
    const char *ptr = "fstatattest.txt";
    struct stat st;
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fstatat_0100", fd >= 0);
    lseek(fd, 0, SEEK_SET);
    int ret = fstatat(AT_FDCWD, ptr, &st, 0);
    EXPECT_EQ("fstatat_0100", ret, SUCCESS);
    uid_t uid = getuid();
    EXPECT_EQ("fstatat_0100", st.st_uid, uid);
    gid_t gid = getgid();
    EXPECT_EQ("fstatat_0100", st.st_gid, gid);
    close(fd);
    remove(ptr);
    ptr = NULL;
}

/**
 * @tc.name      : fstatat_0200
 * @tc.desc      : The parameter fd is equal to AT_FDCWD, the flag is AT_SYMLINK_NOFOLLOW,
 *                 and the information of the file can be obtained.
 * @tc.level     : Level 0
 */
void fstatat_0200(void)
{
    const char *ptr = "fstatattest.txt";
    struct stat st;
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fstatat_0200", fd >= 0);
    lseek(fd, 0, SEEK_SET);
    const char *softptr = "./fstatattest.txt.soft";
    int link = symlink(ptr, softptr);
    EXPECT_EQ("fstatat_0200", link, 0);
    int ret = fstatat(AT_FDCWD, softptr, &st, AT_SYMLINK_NOFOLLOW);
    EXPECT_EQ("fstatat_0200", ret, SUCCESS);
    uid_t uid = getuid();
    EXPECT_EQ("fstatat_0200", st.st_uid, uid);
    gid_t gid = getgid();
    EXPECT_EQ("fstatat_0200", st.st_gid, gid);
    EXPECT_TRUE("fstatat_0200", st.st_mode);
    close(fd);
    remove(softptr);
    remove(ptr);
    softptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : fstatat_0300
 * @tc.desc      : The parameter fd is equal to the return value of opening the test file, the flag is 0,
 *                 and the information of the file can be obtained.
 * @tc.level     : Level 0
 */
void fstatat_0300(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("fstatattest.txt", ptr);

    struct stat st;
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fstatat_0300", fd >= 0);
    lseek(fd, 0, SEEK_SET);
    int ret = fstatat(0, ptr, &st, 0);
    EXPECT_EQ("fstatat_0300", ret, SUCCESS);
    uid_t uid = getuid();
    EXPECT_EQ("fstatat_0300", st.st_uid, uid);
    gid_t gid = getgid();
    EXPECT_EQ("fstatat_0300", st.st_gid, gid);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : fstatat_time64_0100
 * @tc.desc      : The parameter fd is equal to AT_FDCWD, the flag is 0,
 *                 and the information of the file can be obtained.
 * @tc.level     : Level 0
 */
void fstatat_time64_0100(void)
{
    const char *ptr = "fstatat_time64_test.txt";
    struct stat st;
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fstatat_0100", fd >= 0);
    lseek(fd, 0, SEEK_SET);
    int ret = __fstatat_time64(AT_FDCWD, ptr, &st, 0);
    EXPECT_EQ("fstatat_0100", ret, SUCCESS);
    uid_t uid = getuid();
    EXPECT_EQ("fstatat_0100", st.st_uid, uid);
    gid_t gid = getgid();
    EXPECT_EQ("fstatat_0100", st.st_gid, gid);
    close(fd);
    remove(ptr);
    ptr = NULL;
}

int main(int argc, char *argv[])
{
    fstatat_0100();
    fstatat_0200();
    fstatat_0300();
    fstatat_time64_0100();
    return t_status;
}
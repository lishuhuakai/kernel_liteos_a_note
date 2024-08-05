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
#include <stdlib.h>
#include <stdio.h>
#include "functionalext.h"
#include "filepath_util.h"

typedef void (*TEST_FUN)();
const int SUCCESS = 0;
const int FAILED = -1;

/**
 * @tc.name      : faccessat_0100
 * @tc.desc      : Test file exists, amode is F_OK.
 * @tc.level     : Level 0
 */
void faccessat_0100(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("faccessat_0100", fd >= 0);
    int isExist = faccessat(fd, ptr, F_OK, 0);
    EXPECT_EQ("faccessat_0100", isExist, SUCCESS);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_0200
 * @tc.desc      : The test file does not exist, amode is F_OK.
 * @tc.level     : Level 2
 */
void faccessat_0200(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = -1;
    int isExist = faccessat(fd, ptr, F_OK, 0);
    EXPECT_EQ("faccessat_0200", isExist, FAILED);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_0300
 * @tc.desc      : The test file has read permission and amode is R_OK.
 * @tc.level     : Level 0
 */
void faccessat_0300(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, 00040);
    EXPECT_TRUE("faccessat_0300", fd >= 0);
    int isRead = faccessat(fd, ptr, R_OK, 0);
    EXPECT_EQ("faccessat_0300", isRead, SUCCESS);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_0400
 * @tc.desc      : The test file has write permission, and amode is W_OK.
 * @tc.level     : Level 0
 */
void faccessat_0400(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, 00020);
    EXPECT_TRUE("faccessat_0400", fd >= 0);
    int isWrite = faccessat(fd, ptr, W_OK, 0);
    EXPECT_EQ("faccessat_0400", isWrite, SUCCESS);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_0500
 * @tc.desc      : The test file has executable permission, amode is X_OK.
 * @tc.level     : Level 0
 */
void faccessat_0500(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, 00010);
    char cmd[256] = {0};
    snprintf(cmd, sizeof(cmd), "chmod 777 %s", ptr);
    system(cmd);
    EXPECT_TRUE("faccessat_0500", fd >= 0);
    int isExecute = faccessat(fd, ptr, X_OK, 0);
    EXPECT_EQ("faccessat_0500", isExecute, SUCCESS);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_0600
 * @tc.desc      : The test file has no executable permission, amode is X_OK.
 * @tc.level     : Level 2
 */
void faccessat_0600(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("faccessat_0600", fd >= 0);
    int isExecute = faccessat(fd, ptr, X_OK, 0);
    EXPECT_EQ("faccessat_0600", isExecute, FAILED);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_0700
 * @tc.desc      : The test file has read, write and executable permissions, and amode is R_OK|W_OK|X_Ok.
 * @tc.level     : Level 0
 */
void faccessat_0700(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, 00070);
    EXPECT_TRUE("faccessat_0700", fd >= 0);
    int isExecute = faccessat(fd, ptr, R_OK | W_OK | X_OK, 0);
    EXPECT_EQ("faccessat_0700", isExecute, SUCCESS);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_0800
 * @tc.desc      : The test file does not have read/write executable permission, and amode is R_OK|W_OK|X_OK.
 * @tc.level     : Level 2
 */
void faccessat_0800(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("faccessat_0800", fd >= 0);
    int isExecute = faccessat(fd, ptr, R_OK | W_OK | X_OK, 0);
    EXPECT_EQ("faccessat_0800", isExecute, FAILED);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_0900
 * @tc.desc      : The test file has read permissions whith invalid flag parameter (0), and amode is R_OK.
 * @tc.level     : Level 2
 */
void faccessat_0900(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, 00070);
    EXPECT_TRUE("faccessat_0900", fd >= 0);
    int ret = faccessat(fd, ptr, R_OK, 0);
    EXPECT_EQ("faccessat_0900", ret, 0);
    EXPECT_EQ("faccessat_0900", errno, 13);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_1000
 * @tc.desc      : The test file has read, write and executable permissions, fd is AT_FDCWD,
 *                 and amode is R_OK|W_OK|X_OK.
 * @tc.level     : Level 0
 */
void faccessat_1000(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, 00070);
    EXPECT_TRUE("faccessat_1000", fd >= 0);
    int isExecute = faccessat(AT_FDCWD, ptr, R_OK | W_OK | X_OK, AT_EACCESS);
    EXPECT_EQ("faccessat_1000", isExecute, SUCCESS);
    close(fd);
    remove(ptr);
}

/**
 * @tc.name      : faccessat_1100
 * @tc.desc      : The test file does not have read, write and execute permissions, fd is AT_FDCWD,
 *                 and amode is R_OK|W_OK|X_OK.
 * @tc.level     : Level 2
 */
void faccessat_1100(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FACCESSAT_TEST_TXT, ptr);
    int fd = open(ptr, O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("faccessat_1100", fd >= 0);
    int isExecute = faccessat(AT_FDCWD, ptr, R_OK | W_OK | X_OK, AT_EACCESS);
    EXPECT_EQ("faccessat_1100", isExecute, FAILED);
    close(fd);
    remove(ptr);
}

TEST_FUN G_Fun_Array[] = {
    faccessat_0100,
    faccessat_0200,
    faccessat_0300,
    faccessat_0400,
    faccessat_0500,
    faccessat_0600,
    faccessat_0700,
    faccessat_0800,
    faccessat_0900,
    faccessat_1000,
    faccessat_1100,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
/**
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

#include <fcntl.h>
#include <unistd.h>
#include "functionalext.h"

#define TEST_BUFFER_SIZE 4

typedef void (*TEST_FUN)();
const int FILE_ZERO = 0;
const int FILE_NEGA = -1;
const int FILE_EIGHT = 8;
const int FILE_TEN = 10;
const int FILE_TWENTWO = 22;
const int FILE_THIRTYTHREE = 33;

/**
 * @tc.name      : lseek64_0100
 * @tc.desc      : Determine whether the file read position is as expected
 * @tc.level     : Level 2
 */
void lseek64_0100(void)
{
    off64_t DataArry[TEST_BUFFER_SIZE] = {1, 2, 4, 8};
    off64_t offset = 0;
    int fd = open("lseek64_function_test.c", O_RDWR | O_CREAT, TEST_MODE);
    if (fd < 0) {
        t_error("open file failed\n");
        return;
    }
    for (int i = 0; i < TEST_BUFFER_SIZE; i++) {
        lseek64(fd, 0, SEEK_SET);
        offset = lseek64(fd, DataArry[i], SEEK_SET);
        if (offset != DataArry[i]) {
            EXPECT_FALSE("lseek64_0100", 1);
        }
    }
    close(fd);
    int ret = access("lseek64_function_test.c", F_OK);
    if (ret != -1) {
        ret = remove("./lseek64_function_test.c");
    }
}

/**
 * @tc.name      : lseek_0100
 * @tc.desc      : Verify that the file pointer is moved to the beginning of the file
 * @tc.level     : Level 0
 */
void lseek_0100(void)
{
    char *wrstring = "This is a test sample!";
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_STRLT("lseek_0100", 0, fd);
    int retwrite = write(fd, wrstring, sizeof(wrstring));
    off_t data = lseek(fd, 0L, SEEK_SET);
    EXPECT_EQ("lseek_0100", (int)data, FILE_ZERO);
    close(fd);
    remove("/data/readtest.txt");
}

/**
 * @tc.name      : lseek_0200
 * @tc.desc      : Verify that the file pointer is moved anywhere in the file
 * @tc.level     : Level 0
 */
void lseek_0200(void)
{
    char *wrstring = "This is a test sample!";
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_STRLT("lseek_0200", 0, fd);
    int retwrite = write(fd, wrstring, sizeof(wrstring));
    off_t data = lseek(fd, 8L, SEEK_SET);
    EXPECT_EQ("lseek_0200", (int)data, FILE_EIGHT);
    close(fd);
    remove("/data/readtest.txt");
}

/**
 * @tc.name      : lseek_0300
 * @tc.desc      : Verify moves the file pointer to the current file position
 * @tc.level     : Level 0
 */
void lseek_0300(void)
{
    char *wrstring = "This is a test sample!";
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_STRLT("lseek_0300", 0, fd);
    int retwrite = write(fd, wrstring, sizeof(wrstring));
    off_t data = lseek(fd, 10L, SEEK_SET);
    data = lseek(fd, 0L, SEEK_CUR);
    EXPECT_EQ("lseek_0300", (int)data, FILE_TEN);
    close(fd);
    remove("/data/readtest.txt");
}

/**
 * @tc.name      : lseek_0400
 * @tc.desc      : Verify that the file pointer is moved to the end of the file
 * @tc.level     : Level 0
 */
void lseek_0400(void)
{
    char str[] = "This is a test sample!";
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_STRLT("lseek_0400", 0, fd);
    int retwrite = write(fd, str, sizeof(str));
    off_t data = lseek(fd, -1L, SEEK_END);
    EXPECT_EQ("lseek_0400", (int)data, FILE_TWENTWO);
    close(fd);
    remove("/data/readtest.txt");
}

/**
 * @tc.name      : lseek_0500
 * @tc.desc      : Verify that the move file pointer position is past the start position
 * @tc.level     : Level 2
 */
void lseek_0500(void)
{
    char str[] = "This is a test sample!";
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_STRLT("lseek_0500", 0, fd);
    int retwrite = write(fd, str, sizeof(str));
    off_t data = lseek(fd, -10L, SEEK_SET);
    EXPECT_EQ("lseek_0500", (int)data, FILE_NEGA);
    close(fd);
    remove("/data/readtest.txt");
}

/**
 * @tc.name      : lseek_0600
 * @tc.desc      : Verify that the moved file pointer position is past the end position
 * @tc.level     : Level 1
 */
void lseek_0600(void)
{
    char str[] = "This is a test sample!";
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_STRLT("lseek_0600", 0, fd);
    int retwrite = write(fd, str, sizeof(str));
    off_t data = lseek(fd, 10L, SEEK_END);
    EXPECT_EQ("lseek_0600", (int)data, FILE_THIRTYTHREE);
    close(fd);
    remove("/data/readtest.txt");
}

TEST_FUN G_Fun_Array[] = {
    lseek64_0100,
    lseek_0100,
    lseek_0200,
    lseek_0300,
    lseek_0400,
    lseek_0500,
    lseek_0600,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
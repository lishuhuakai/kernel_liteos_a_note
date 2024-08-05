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

#include <unistd.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "functionalext.h"

#define BUFFER_SIZE 8

/**
 * @tc.name      : preadv_0100
 * @tc.desc      : Create and open the preadv_function_value.c file, write data to the file, call the preadv function
 *                 to read the data in the file from the beginning of the file, close and delete the created file
 * @tc.level     : Level 1
 */
void preadv_0100(void)
{
    char buf1[BUFFER_SIZE];
    char buf2[BUFFER_SIZE];
    struct iovec iov[2];

    memset(buf1, '\0', BUFFER_SIZE);
    memset(buf2, '\0', BUFFER_SIZE);

    iov[0].iov_base = buf1;
    iov[0].iov_len = sizeof(buf1) / sizeof(char);
    iov[1].iov_base = buf2;
    iov[1].iov_len = sizeof(buf2) / sizeof(char);

    int fd = open("./preadv_function_value.c", O_RDWR | O_CREAT, TEST_MODE);
    if (fd == -1) {
        EXPECT_NE("preadv_0100", fd, ERREXPECT);
        return;
    }
    char data[] = "preadv test";
    int arraysize = sizeof(data) / sizeof(char);
    int ret = write(fd, data, sizeof(data));
    if (ret < 0) {
        EXPECT_NE("preadv_0100", ret, EOF);
        return;
    }
    int count = sizeof(iov) / sizeof(struct iovec);
    ret = preadv(fd, iov, count, 0);
    EXPECT_EQ("preadv_0100", ret, arraysize);

    close(fd);
    ret = access("preadv_function_value.c", F_OK);
    if (ret != -1) {
        ret = remove("./preadv_function_value.c");
        EXPECT_EQ("preadv_0100", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : preadv_0200
 * @tc.desc      : Pass in the abnormal file descriptor and call the preadv function to read the data(failed)
 * @tc.level     : Level 2
 */
void preadv_0200(void)
{
    char buf1[8];
    char buf2[8];
    struct iovec iov[2];

    iov[0].iov_base = buf1;
    iov[0].iov_len = sizeof(buf1) / sizeof(char);
    iov[1].iov_base = buf2;
    iov[1].iov_len = sizeof(buf2) / sizeof(char);
    int count = sizeof(iov) / sizeof(struct iovec);
    int ret = preadv(-1, iov, count, 0);
    EXPECT_EQ("preadv_0200", ret, EOF);
}

int main(void)
{
    preadv_0100();
    preadv_0200();
    return t_status;
}

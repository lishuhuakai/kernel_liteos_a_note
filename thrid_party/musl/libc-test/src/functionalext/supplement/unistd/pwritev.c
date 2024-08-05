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

/**
 * @tc.name      : pwritev_0100
 * @tc.desc      : Create a temporary file and centrally write the contents of multiple buffers to the file(success)
 * @tc.level     : Level 0
 */
void pwritev_0100(void)
{
    char buf1[] = "preadv";
    char buf2[] = "and pwritev";
    struct iovec iov[2];

    iov[0].iov_base = buf1;
    iov[0].iov_len = sizeof(buf1) / sizeof(char);
    iov[1].iov_base = buf2;
    iov[1].iov_len = sizeof(buf2) / sizeof(char);

    int fd = open("pwritev_function_value.c", O_RDWR | O_CREAT, TEST_MODE);
    int count = sizeof(iov) / sizeof(struct iovec);
    int ret = pwritev(fd, iov, count, 0);
    EXPECT_NE("pwritev_0100", ret, EOF);

    close(fd);
    ret = access("pwritev_function_value.c", F_OK);
    if (ret == 0) {
        ret = remove("pwritev_function_value.c");
        EXPECT_EQ("pwritev_0100", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : pwritev_0200
 * @tc.desc      : Create a temporary file and centrally write the contents of multiple buffers to the file(failed)
 * @tc.level     : Level 2
 */
void pwritev_0200(void)
{
    char buf1[] = "pwritev1";
    char buf2[] = "pwritev2";
    struct iovec iov[2];

    iov[0].iov_base = buf1;
    iov[0].iov_len = sizeof(buf1) / sizeof(char);
    iov[1].iov_base = buf2;
    iov[1].iov_len = sizeof(buf2) / sizeof(char);

    int fd = open("pwritev_function_value.c", O_RDWR | O_CREAT, TEST_MODE);
    if (fd == -1) {
        EXPECT_NE("pwritev_0200", fd, ERREXPECT);
        return;
    }
    int count = sizeof(iov) / sizeof(struct iovec);
    int ret = pwritev(fd, iov, count + 100, 0);
    EXPECT_EQ("pwritev_0200", ret, EOF);

    close(fd);
    ret = access("pwritev_function_value.c", F_OK);
    if (ret == 0) {
        ret = remove("pwritev_function_value.c");
        EXPECT_EQ("pwritev_0200", ret, CMPFLAG);
    }
}

/**
 * @tc.name      : pwritev_0300
 * @tc.desc      : Write multiple buffer contents to invalid file descriptor
 * @tc.level     : Level 2
 */
void pwritev_0300(void)
{
    char buf1[] = "pwritev1";
    char buf2[] = "pwritev2";
    struct iovec iov[2];

    iov[0].iov_base = buf1;
    iov[0].iov_len = sizeof(buf1) / sizeof(char);
    iov[1].iov_base = buf2;
    iov[1].iov_len = sizeof(buf2) / sizeof(char);
    int count = sizeof(iov) / sizeof(struct iovec);
    int ret = pwritev(-1, iov, count, 0);
    EXPECT_EQ("pwritev_0300", ret, EOF);
}

int main(void)
{
    pwritev_0100();
    pwritev_0200();
    pwritev_0300();
    return t_status;
}

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

#include <errno.h>
#include <stdlib.h>
#include <sys/uio.h>
#include "functionalext.h"

#define __VALUE_BUFFER_SIZE__ 256
#define __VALUE_DATA_SIZE__ 2

/**
 * @tc.name      : process_vm_writev_0100
 * @tc.desc      : Single data inter-process data transfer
 * @tc.level     : Level 0
 */
void process_vm_writev_0100(void)
{
    char src[__VALUE_BUFFER_SIZE__] = "This is process_vm_writev_0100.";
    char dst[__VALUE_BUFFER_SIZE__] = "";
    struct iovec local = {.iov_base = dst, .iov_len = sizeof(dst)};
    struct iovec remote = {.iov_base = src, .iov_len = sizeof(src)};

    ssize_t rev = process_vm_readv(getpid(), &local, 1, &remote, 1, 0);
    EXPECT_EQ("process_vm_writev_0100", rev, sizeof(dst));
    EXPECT_STREQ("process_vm_writev_0100", dst, src);
}

/**
 * @tc.name      : process_vm_writev_0200
 * @tc.desc      : Data transfer between multiple data processes
 * @tc.level     : Level 0
 */
void process_vm_writev_0200(void)
{
    char src1[__VALUE_BUFFER_SIZE__] = "This is process_vm_writev_0200_1.";
    char src2[__VALUE_BUFFER_SIZE__] = "This is process_vm_writev_0200_2.";
    char dst1[__VALUE_BUFFER_SIZE__] = "";
    char dst2[__VALUE_BUFFER_SIZE__] = "";
    struct iovec local[__VALUE_DATA_SIZE__] = {
        {.iov_base = dst1, .iov_len = sizeof(dst1)},
        {.iov_base = dst2, .iov_len = sizeof(dst2)},
    };
    struct iovec remote[__VALUE_DATA_SIZE__] = {
        {.iov_base = src1, .iov_len = sizeof(src1)},
        {.iov_base = src2, .iov_len = sizeof(src2)},
    };

    ssize_t rev = process_vm_readv(getpid(), local, 2, remote, 2, 0);
    EXPECT_EQ("process_vm_writev_0200", rev, sizeof(dst1) + sizeof(dst2));
    int i;
    for (i = 0; i < __VALUE_DATA_SIZE__; i++) {
        EXPECT_STREQ("process_vm_writev_0200", local[i].iov_base, remote[i].iov_base);
    }
}

/**
 * @tc.name      : process_vm_writev_0300
 * @tc.desc      : Provide the correct template, create a temporary file
 * @tc.level     : Level 2
 */
void process_vm_writev_0300(void)
{
    char src[__VALUE_BUFFER_SIZE__] = "This is process_vm_writev_0300.";
    struct iovec local = {
        .iov_base = NULL,
        .iov_len = 0,
    };
    struct iovec remote = {.iov_base = src, .iov_len = sizeof(src)};

    ssize_t rev = process_vm_readv(getpid(), &local, 1, &remote, 1, 0);
    EXPECT_EQ("process_vm_writev_0300", rev, 0);
}

/**
 * @tc.name      : process_vm_readv_0100
 * @tc.desc      : Transfers data from the remote process to the local process
 * @tc.level     : Level 0
 */
void process_vm_readv_0100(void)
{
    char src[__VALUE_BUFFER_SIZE__] = "This is process_vm_readv_0100.";
    char dst[__VALUE_BUFFER_SIZE__] = "";
    struct iovec remote, local;
    remote.iov_base = src;
    remote.iov_len = __VALUE_BUFFER_SIZE__;

    local.iov_base = dst;
    local.iov_len = __VALUE_BUFFER_SIZE__;

    int ret = process_vm_readv(getpid(), &local, 1, &remote, 1, 0);
    EXPECT_EQ("process_vm_readv_0100", ret, sizeof(src));
    EXPECT_STREQ("process_vm_readv_0100", dst, src);
}

/**
 * @tc.name      : process_vm_readv_0200
 * @tc.desc      : No remote instance data specified, transfer data from remote process to local process
 * @tc.level     : Level 2
 */
void process_vm_readv_0200(void)
{
    char dst[__VALUE_BUFFER_SIZE__] = "";
    struct iovec remote, local;
    remote.iov_base = NULL;
    remote.iov_len = __VALUE_BUFFER_SIZE__;

    local.iov_base = dst;
    local.iov_len = __VALUE_BUFFER_SIZE__;

    int ret = process_vm_readv(getpid(), &local, 1, &remote, 1, 0);
    EXPECT_EQ("process_vm_readv_0200", ret, ERREXPECT);
    EXPECT_EQ("process_vm_readv_0200", errno, EFAULT);
}

/**
 * @tc.name      : process_vm_readv_0300
 * @tc.desc      : No local instance data specified, transfer data from remote process to local process
 * @tc.level     : Level 2
 */
void process_vm_readv_0300(void)
{
    char src[__VALUE_BUFFER_SIZE__] = "This is process_vm_readv_0300.";
    struct iovec remote, local;
    remote.iov_base = src;
    remote.iov_len = __VALUE_BUFFER_SIZE__;

    local.iov_base = NULL;
    local.iov_len = __VALUE_BUFFER_SIZE__;

    int ret = process_vm_readv(getpid(), &local, 1, &remote, 1, 0);
    EXPECT_EQ("process_vm_readv_0300", ret, ERREXPECT);
    EXPECT_EQ("process_vm_readv_0300", errno, EFAULT);
}

int main(void)
{
    process_vm_writev_0100();
    process_vm_writev_0200();
    process_vm_writev_0300();
    process_vm_readv_0100();
    process_vm_readv_0200();
    process_vm_readv_0300();
    return t_status;
}
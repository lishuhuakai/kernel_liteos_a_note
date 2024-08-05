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

const int FAILED = -1;

/**
 * @tc.name     : dup3_0100
 * @tc.desc     : The parameter flags is O_CLOEXEC, which can assign the old file descriptor to the new file descriptor
 * @tc.level    : Level 0
 */
void dup3_0100(void)
{
    int fd = open("/data/dup3test.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("dup3_0100", fd >= 0);
    int ret = dup3(fd, fileno(stderr), O_CLOEXEC);
    EXPECT_EQ("dup3_0100", ret, fileno(stderr));
    close(fd);
    remove("/data/dup3test.txt");
}

/**
 * @tc.name     : dup3_0200
 * @tc.desc     : The parameter old is invalid, the old file descriptor cannot be assigned to the new file descriptor.
 * @tc.level    : Level 2
 */
void dup3_0200(void)
{
    int fd = -1;
    int ret = dup3(fd, fileno(stderr), O_CLOEXEC);
    EXPECT_EQ("dup3_0200", ret, FAILED);
}

/**
 * @tc.name     : dup3_0300
 * @tc.desc     : The parameter new is invalid, the old file descriptor cannot be assigned to the new file descriptor.
 * @tc.level    : Level 2
 */
void dup3_0300(void)
{
    int newfd = -1;
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("dup3_0300", fd >= 0);
    int ret = dup3(fd, newfd, O_CLOEXEC);
    EXPECT_EQ("dup3_0300", ret, FAILED);
    close(fd);
    remove("/data/readtest.txt");
}

int main(int argc, char *argv[])
{
    dup3_0100();
    dup3_0200();
    dup3_0300();
    return t_status;
}
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

#include <fcntl.h>
#include "functionalext.h"

const int SIZE = 18;
const int SUCCESS = 0;
const int FAILED = -1;

/**
 * @tc.name      : fallocate_0100
 * @tc.desc      : The parameter mode is 0, which can expand the file space.
 * @tc.level     : Level 0
 */
void fallocate_0100()
{
    char str[] = "this is a fatest\n";
    char buffer[1024] = {0};
    int fd = open("fatest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fallocate_0100", fd >= 0);
    int retwrite = write(fd, str, sizeof(str));
    EXPECT_EQ("fallocate_0100", retwrite, SIZE);
    int ret = fallocate(fd, 0, 4096, 4096 * 3);
    EXPECT_EQ("fallocate_0100", ret, SUCCESS);
    close(fd);
    remove("fatest.txt");
}

/**
 * @tc.name      : fallocate_0200
 * @tc.desc      : The parameter mode is FALLOC_FL_KEEP_SIZE, which can expand the file space.
 * @tc.level     : Level 1
 */
void fallocate_0200(void)
{
    char str[] = "this is a fatest\n";
    char buffer[1024] = {0};
    int fd = open("fatest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fallocate_0200", fd >= 0);
    int retwrite = write(fd, str, sizeof(str));
    EXPECT_EQ("fallocate_0200", retwrite, SIZE);
    int ret = fallocate(fd, FALLOC_FL_KEEP_SIZE, 4096, 4096 * 3);
    EXPECT_EQ("fallocate_0200", ret, SUCCESS);
    close(fd);
    remove("fatest.txt");
}

/**
 * @tc.name      : fallocate_0300
 * @tc.desc      : The parameter mode is FALLOC_FL_KEEP_SIZE|FALLOC_FL_PUNCH_HOLE, which can release file space.
 * @tc.level     : Level 1
 */
void fallocate_0300(void)
{
    char str[] = "this is a fatest\n";
    char buffer[1024] = {0};
    int fd = open("fatest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("fallocate_0300", fd >= 0);
    int retwrite = write(fd, str, sizeof(str));
    EXPECT_EQ("fallocate_0300", retwrite, SIZE);
    lseek(fd, 0, SEEK_SET);
    int ret = fallocate(fd, FALLOC_FL_KEEP_SIZE | FALLOC_FL_PUNCH_HOLE, 4096, 4096 * 3);
    EXPECT_EQ("fallocate_0300", ret, SUCCESS);
    close(fd);
    remove("fatest.txt");
}

/**
 * @tc.name      : fallocate_0400
 * @tc.desc      : The parameter fd is invalid, the mode is FALLOC_FL_KEEP_SIZE, and the file space cannot be expanded
 * @tc.level     : Level 1
 */
void fallocate_0400(void)
{
    int fd = -1;
    int ret = fallocate(fd, FALLOC_FL_KEEP_SIZE, 4096, 4096 * 3);
    EXPECT_EQ("fallocate_0400", ret, FAILED);
    close(fd);
    remove("fatest.txt");
}

int main(int argc, char *argv[])
{
    fallocate_0100();
    fallocate_0200();
    fallocate_0300();
    fallocate_0400();
    return t_status;
}
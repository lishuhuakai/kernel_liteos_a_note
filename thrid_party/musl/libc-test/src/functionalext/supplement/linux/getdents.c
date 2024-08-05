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

#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include "functionalext.h"

const int FAILED = -1;

/**
 * @tc.name      : getdents_0100
 * @tc.desc      : Each parameter is valid, the len parameter is less than or equal to INT_MAX, and the directory
 *                 information can be read into the specified buffer.
 * @tc.level     : Level 0
 */
void getdents_0100(void)
{
    struct dirent buf;
    int fd = open("/data/data", O_RDONLY);
    EXPECT_NE("getdents_0100", fd, -1);
    int result = getdents(fd, &buf, INT_MAX);
    EXPECT_TRUE("getdents_0100", result > 0);
}

/**
 * @tc.name      : getdents_0200
 * @tc.desc      : The fd parameter is invalid, the directory information cannot be read into the specified buffer.
 * @tc.level     : Level 2
 */
void getdents_0200(void)
{
    struct dirent buf;
    int fd = open("/getdents", O_RDONLY);
    int result = getdents(fd, &buf, INT_MAX);
    EXPECT_EQ("getdents_0200", result, FAILED);
}

/**
 * @tc.name      : getdents_0300
 * @tc.desc      : The buf parameter is invalid, the directory information cannot be read into the specified buffer.
 * @tc.level     : Level 2
 */
void getdents_0300(void)
{
    struct dirent buf;
    int fd = open("/data/data", O_RDONLY);
    EXPECT_NE("getdents_0300", fd, -1);
    int result = getdents(fd, NULL, INT_MAX);
    EXPECT_EQ("getdents_0300", result, FAILED);
}

int main(int argc, char *argv[])
{
    getdents_0100();
    getdents_0200();
    getdents_0300();
    return t_status;
}
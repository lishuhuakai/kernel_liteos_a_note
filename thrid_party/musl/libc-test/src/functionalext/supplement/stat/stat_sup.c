/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "functionalext.h"

const int32_t CREAT_MODE = 666;

/**
 * @tc.name      : stat_0100
 * @tc.desc      : Verify stat process success with two params
 * @tc.level     : Level 0
 */
void stat_0100(void)
{
    struct stat buf;
    int fd = -1;
    fd = creat("test1.txt", CREAT_MODE);
    if (fd < 0) {
        EXPECT_MT("stat_0100", fd, 0);
        return;
    } else {
        close(fd);
    }
    int32_t ret = stat("test1.txt", &buf);
    EXPECT_EQ("stat_0100", ret, 0);
    EXPECT_EQ("stat_0100", buf.st_size, 0);
    ret = remove("test1.txt");
    EXPECT_EQ("stat_0100", ret, 0);
}

/**
 * @tc.name      : stat_0200
 * @tc.desc      : Verify stat process fail because file is not exit
 * @tc.level     : Level 2
 */
void stat_0200(void)
{
    struct stat buf;
    int32_t ret = stat("test2.txt", &buf);
    EXPECT_EQ("stat_0200", ret, -1);
}

int main(void)
{
    stat_0100();
    stat_0200();
    return t_status;
}

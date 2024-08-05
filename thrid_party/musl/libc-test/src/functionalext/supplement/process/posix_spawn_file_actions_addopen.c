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

#include <sys/mman.h>
#include <spawn.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fdop.h>
#include "functionalext.h"

/**
 * @tc.name      : posix_spawn_file_actions_addopen_0100
 * @tc.desc      : Set the behavior property of the open file to the spawn file actions object
 * @tc.level     : Level 0
 */
void posix_spawn_file_actions_addopen_0100(void)
{
    posix_spawn_file_actions_t fa;
    struct fdop *ptr = NULL;

    posix_spawn_file_actions_init(&fa);

    int fd = 10;
    int ret = posix_spawn_file_actions_addopen(&fa, fd, "/data", O_RDWR | O_CREAT, S_IXGRP);
    EXPECT_EQ("posix_spawn_file_actions_addopen_0100", ret, CMPFLAG);
    if (ret == 0) {
        ptr = (struct fdop *)fa.__actions;
        EXPECT_EQ("posix_spawn_file_actions_addopen_0100", ptr->cmd, FDOP_OPEN);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0100", ptr->fd, fd);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0100", ptr->oflag, O_RDWR | O_CREAT);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0100", ptr->mode, S_IXGRP);
        EXPECT_STREQ("posix_spawn_file_actions_addopen_0100", ptr->path, "/data");
    }

    posix_spawn_file_actions_destroy(&fa);
}

/**
 * @tc.name      : posix_spawn_file_actions_addopen_0200
 * @tc.desc      : Set the behavior property of the open file to the spawn file actions object
 * @tc.level     : Level 0
 */
void posix_spawn_file_actions_addopen_0200(void)
{
    posix_spawn_file_actions_t fa;
    struct fdop *ptr = NULL;
    int fd = 10;
    posix_spawn_file_actions_init(&fa);
    int ret = posix_spawn_file_actions_addopen(&fa, fd, "/data", O_RDONLY | O_CREAT, S_IXGRP | S_IROTH | S_IWOTH);
    EXPECT_EQ("posix_spawn_file_actions_addopen_0200", ret, CMPFLAG);
    if (ret == 0) {
        ptr = (struct fdop *)fa.__actions;
        EXPECT_EQ("posix_spawn_file_actions_addopen_0200", ptr->cmd, FDOP_OPEN);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0200", ptr->fd, fd);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0200", ptr->oflag, O_RDONLY | O_CREAT);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0200", ptr->mode, S_IXGRP | S_IROTH | S_IWOTH);
        EXPECT_STREQ("posix_spawn_file_actions_addopen_0200", ptr->path, "/data");
    }

    ret = posix_spawn_file_actions_destroy(&fa);
}

/**
 * @tc.name      : posix_spawn_file_actions_addopen_0300
 * @tc.desc      : Set the behavior property of the open file to the spawn file actions object
 * @tc.level     : Level 0
 */
void posix_spawn_file_actions_addopen_0300(void)
{
    posix_spawn_file_actions_t fa;
    struct fdop *ptr = NULL;
    int fd = 10;
    posix_spawn_file_actions_init(&fa);
    int ret = posix_spawn_file_actions_addopen(&fa, fd, "/data", O_WRONLY | O_CREAT, S_IXGRP | S_IROTH | S_IWOTH);
    EXPECT_EQ("posix_spawn_file_actions_addopen_0300", ret, CMPFLAG);
    if (ret == 0) {
        ptr = (struct fdop *)fa.__actions;
        EXPECT_EQ("posix_spawn_file_actions_addopen_0300", ptr->cmd, FDOP_OPEN);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0300", ptr->fd, fd);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0300", ptr->oflag, O_WRONLY | O_CREAT);
        EXPECT_EQ("posix_spawn_file_actions_addopen_0300", ptr->mode, S_IXGRP | S_IROTH | S_IWOTH);
        EXPECT_STREQ("posix_spawn_file_actions_addopen_0300", ptr->path, "/data");
    }

    ret = posix_spawn_file_actions_destroy(&fa);
}

int main(void)
{
    posix_spawn_file_actions_addopen_0100();
    posix_spawn_file_actions_addopen_0200();
    posix_spawn_file_actions_addopen_0300();
    return t_status;
}

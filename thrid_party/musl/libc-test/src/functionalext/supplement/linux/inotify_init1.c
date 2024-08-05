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

#include <sys/inotify.h>
#include "functionalext.h"

/**
 * @tc.name      : inotify_init1_0100
 * @tc.desc      : Verify inotify_init1 process success. when param flag is 0
 * @tc.level     : Level 0
 */
void inotify_init1_0100(void)
{
    errno = 0;
    int fd = inotify_init1(0);
    EXPECT_NE("inotify_init1_0100", fd, -1);
    EXPECT_EQ("inotify_init1_0100", errno, 0);
    close(fd);
}

/**
 * @tc.name      : inotify_init1_0200
 * @tc.desc      : Verify inotify_init1 process success. when param flag is IN_NONBLOCK
 * @tc.level     : Level 0
 */
void inotify_init1_0200(void)
{
    errno = 0;
    int fd = inotify_init1(IN_NONBLOCK);
    EXPECT_NE("inotify_init1_0200", fd, -1);
    EXPECT_EQ("inotify_init1_0200", errno, 0);
    close(fd);
}

/**
 * @tc.name      : inotify_init1_0300
 * @tc.desc      : Verify inotify_init1 process success. when param flag is IN_CLOEXEC
 * @tc.level     : Level 0
 */
void inotify_init1_0300(void)
{
    errno = 0;
    int fd = inotify_init1(IN_CLOEXEC);
    EXPECT_NE("inotify_init1_0300", fd, -1);
    EXPECT_EQ("inotify_init1_0300", errno, 0);
    close(fd);
}

/**
 * @tc.name      : inotify_init1_0400
 * @tc.desc      : Verify inotify_init1 process success. when param flag is invalid
 * @tc.level     : Level 2
 */
void inotify_init1_0400(void)
{
    errno = 0;
    int fd = inotify_init1(-1);
    EXPECT_EQ("inotify_init1_0400", fd, -1);
    EXPECT_EQ("inotify_init1_0400", errno, EINVAL);
    close(fd);
}

int main(void)
{
    inotify_init1_0100();
    inotify_init1_0200();
    inotify_init1_0300();
    inotify_init1_0400();
    return t_status;
}

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
#include <sys/xattr.h>
#include "functionalext.h"

const char *path = "/data/test.txt";

/**
 * @tc.name      : fremovexattr_0100
 * @tc.desc      : Verify that the extended properties of the specified file can be deleted.
 * @tc.level     : Level 0
 */
void fremovexattr_0100(void)
{
    char buf[10];

    int fd = open(path, O_CREAT | O_WRONLY, 0667);
    EXPECT_NE("fremovexattr_0100", fd, -1);

    int result = fsetxattr(fd, "user.foo", "bar", 4, 0);
    EXPECT_EQ("fremovexattr_0100", result, 0);

    result = fgetxattr(fd, "user.foo", buf, sizeof(buf));
    EXPECT_EQ("fremovexattr_0100", result, 4);
    EXPECT_STREQ("fremovexattr_0100", buf, "bar");

    result = fremovexattr(fd, "user.foo");
    EXPECT_EQ("fremovexattr_0100", result, 0);

    memset(buf, 0, sizeof(buf));
    result = fgetxattr(fd, "user.foo", buf, sizeof(buf));
    EXPECT_EQ("fremovexattr_0100", result, -1);

    close(fd);
    remove(path);
}

int main(int argc, char *argv[])
{
    fremovexattr_0100();
    return t_status;
}
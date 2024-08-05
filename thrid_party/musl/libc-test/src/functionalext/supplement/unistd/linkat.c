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

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

const int32_t CREAT_MODE = 666;

/**
 * @tc.name      : linkat_0100
 * @tc.desc      : Vertify linkat process success.
 * @tc.level     : Level 0
 */
void linkat_0100(void)
{
    int fd1 = creat("/etc/test.txt", CREAT_MODE);
    int fd2 = -1;
    if (fd1 < 0) {
        EXPECT_MT("linkat_0100", fd1, 0);
        return;
    } else {
        close(fd1);
    }
    int result = linkat(fd1, "/etc/test.txt", fd2, "/etc/linkat.txt", AT_SYMLINK_FOLLOW);
    EXPECT_EQ("linkat_0100", result, 0);
    EXPECT_EQ("linkat_0100", access("/etc/linkat.txt", F_OK), 0);
    system("rm -f /etc/linkat.txt");
}

/**
 * @tc.name      : linkat_0200
 * @tc.desc      : Vertify linkat process fail. Beceause creating a hard linkat file with the same name.
 * @tc.level     : Level 2
 */
void linkat_0200(void)
{
    int fd1 = creat("/etc/test.txt", CREAT_MODE);
    int fd2 = -1;
    if (access("/etc/linkat.txt", F_OK) != 0) {
        system("touch /etc/test.txt");
        if (fd1 < 0) {
            EXPECT_MT("linkat_0100", fd1, 0);
            return;
        } else {
            close(fd1);
        }
        int result = linkat(fd1, "/etc/test.txt", fd2, "/etc/linkat.txt", AT_SYMLINK_FOLLOW);
        EXPECT_EQ("linkat_0200", result, 0);
    }
    EXPECT_NE("linkat_0200", linkat(fd1, "/etc/test.txt", fd2, "/etc/linkat.txt", AT_SYMLINK_FOLLOW), 0);
    EXPECT_EQ("linkat_0200", errno, EEXIST);
    system("rm -f /etc/linkat.txt");
}

/**
 * @tc.name      : linkat_0300
 * @tc.desc      : Vertify linkat process fail. Beceause param is invalid.
 * @tc.level     : Level 2
 */
void linkat_0300(void)
{
    int result = linkat(-1, NULL, -1, NULL, -1);
    EXPECT_EQ("linkat_0300", result, ERREXPECT);
    EXPECT_EQ("linkat_0300", errno, EINVAL);

    if (access("/etc/test.txt", F_OK) == 0) {
        system("rm -f /etc/test.txt");
    }

    if (access("/etc/linkat.txt", F_OK) == 0) {
        system("rm -f /etc/linkat.txt");
    }
}

int main(void)
{
    linkat_0100();
    linkat_0200();
    linkat_0300();
    return t_status;
}
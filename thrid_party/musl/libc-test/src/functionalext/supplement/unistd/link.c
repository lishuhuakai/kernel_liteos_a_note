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

#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : link_0100
 * @tc.desc      : Determine whether the created connection is successful and whether the file exists
 * @tc.level     : Level 0
 */
void link_0100(void)
{
    system("touch /etc/a.txt");
    int result = link("/etc/a.txt", "/etc/a_link.txt");
    EXPECT_EQ("link_0100", result, 0);
    EXPECT_EQ("link_0100", access("/etc/a_link.txt", F_OK), 0);
    system("rm -f /etc/a_link.txt");
}

/**
 * @tc.name      : link_0200
 * @tc.desc      : Determine whether the file is successfully created when creating a hard link file with the same name
 * @tc.level     : Level 2
 */
void link_0200(void)
{
    if (access("/etc/a_link.txt", F_OK) != 0) {
        int result = link("/etc/a.txt", "/etc/a_link.txt");
        EXPECT_EQ("link_0200", result, 0);
    }
    EXPECT_NE("link_0200", link("/etc/a.txt", "/etc/a_link.txt"), 0);
    EXPECT_EQ("link_0200", errno, EEXIST);
    system("rm -f /etc/a_link.txt");
}

/**
 * @tc.name      : link_0300
 * @tc.desc      : Outlier judgment
 * @tc.level     : Level 2
 */
void link_0300(void)
{
    int result = link(NULL, NULL);
    EXPECT_EQ("link_0300", result, ERREXPECT);
    EXPECT_EQ("link_0300", errno, EFAULT);

    if (access("/etc/a.txt", F_OK) == 0) {
        system("rm -f /etc/a.txt");
    }

    if (access("/etc/a_link.txt", F_OK) == 0) {
        system("rm -f /etc/a_link.txt");
    }
}

int main(void)
{
    // link_0100();
    // link_0200();
    // link_0300();

    return t_status;
}
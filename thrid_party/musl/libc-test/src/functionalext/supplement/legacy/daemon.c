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
#include <unistd.h>
#include "functionalext.h"

int successfully = 0;
int failed = -1;

/*
 * @tc.name      : daemon_0100
 * @tc.desc      : Verify that the working directory of the daemon can be changed (parameters is 0)
 * @tc.level     : Level 0
 */
void daemon_0100(void)
{
    pid_t pid;
    int result = daemon(0, 0);
    EXPECT_EQ("daemon_0100", result, successfully);
}

/*
 * @tc.name      : daemon_0200
 * @tc.desc      : Verify that the working directory of the daemon can be changed (parameters is 7)
 * @tc.level     : Level 0
 */
void daemon_0200(void)
{
    pid_t pid;
    int result = daemon(7, 0);
    EXPECT_EQ("daemon_0200", result, successfully);
    int fd = open("/dev/null", O_RDWR);
    EXPECT_EQ("daemon_0200", fd, successfully);
}

/*
 * @tc.name      : daemon_0300
 * @tc.desc      : Verify that the working directory of the daemon cannot be changed
 *                 (the nochdir parameter has no effect)
 * @tc.level     : Level 2
 */
void daemon_0300(void)
{
    pid_t pid;
    int result = daemon(0, -1);
    EXPECT_EQ("daemon_0300", result, failed);
}

/*
 * @tc.name      : daemon_0400
 * @tc.desc      : Verify that the working directory of the daemon cannot be changed
 *                 (the noclose parameter has no effect)
 * @tc.level     : Level 2
 */
void daemon_0400(void)
{
    pid_t pid;
    int result = daemon(-1, -1);
    EXPECT_EQ("daemon_0400", result, failed);
    int fd = open("/dev/null", O_RDWR);
    EXPECT_EQ("daemon_0400", fd, failed);
}

/*
 * @tc.name      : daemon_0500
 * @tc.desc      : Verify that the working directory of the daemon cannot be changed (parameter invalid)
 * @tc.level     : Level 2
 */
void daemon_0500(void)
{
    pid_t pid;
    int result = daemon(0, 0);
    EXPECT_EQ("daemon_0500", result, successfully);
    pid = fork();
    EXPECT_EQ("daemon_0500", pid, failed);
}

/*
 * @tc.name      : daemon_0600
 * @tc.desc      : Verify that the working directory of the daemon cannot be changed (parameter invalid)
 * @tc.level     : Level 2
 */
void daemon_0600(void)
{
    pid_t pid;
    int result = daemon(0, 0);
    EXPECT_EQ("daemon_0600", result, successfully);
    int mode = setsid();
    EXPECT_EQ("daemon_0600", mode, failed);
}

int main(int argc, char *argv[])
{
    daemon_0100();
    daemon_0200();
    daemon_0300();
    daemon_0400();
    daemon_0500();
    daemon_0600();

    return t_status;
}
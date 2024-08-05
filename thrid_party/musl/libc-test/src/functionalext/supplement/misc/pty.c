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
#include <pty.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "functionalext.h"

#define TEST_BUFFER_SIZE 128

/**
 * @tc.name      : grantpt_0100
 * @tc.desc      : Grant access to the slave pseudoterminal
 * @tc.level     : Level 0
 */
void grantpt_0100(void)
{
    int rev = grantpt(0);
    EXPECT_EQ("grantpt_0100", rev, 0);
}

/**
 * @tc.name      : ptsname_r_0100
 * @tc.desc      : Provide the correct parameters to get the terminal device name
 * @tc.level     : Level 0
 */
void ptsname_r_0100(void)
{
    char buf[TEST_BUFFER_SIZE];
    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    EXPECT_NE("ptsname_r_0100", fd, -1);
    int ret = grantpt(fd);
    EXPECT_EQ("ptsname_r_0100", ret, 0);

    ret = ptsname_r(fd, buf, sizeof(buf));
    EXPECT_EQ("ptsname_r_0100", ret, 0);
    close(fd);
}

/**
 * @tc.name      : ptsname_r_0200
 * @tc.desc      : Provide illegal buffer parameter, get terminal device name
 * @tc.level     : Level 2
 */
void ptsname_r_0200(void)
{
    char *buf = NULL;
    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    EXPECT_NE("ptsname_r_0200", fd, -1);
    int ret = grantpt(fd);
    EXPECT_EQ("ptsname_r_0200", ret, 0);

    ret = ptsname_r(fd, buf, TEST_BUFFER_SIZE);
    EXPECT_EQ("ptsname_r_0200", ret, ERANGE);
    close(fd);
}

/**
 * @tc.name      : ptsname_r_0300
 * @tc.desc      : Provide wrong device ID number, get terminal device name
 * @tc.level     : Level 2
 */
void ptsname_r_0300(void)
{
    char buf[TEST_BUFFER_SIZE];
    int ret = ptsname_r(STDOUT_FILENO, buf, sizeof(buf));
    EXPECT_EQ("ptsname_r_0300", ret, ENOTTY);
}

/**
 * @tc.name      : ptsname_r_0400
 * @tc.desc      : Provide a small buffer to get the terminal device name
 * @tc.level     : Level 2
 */
void ptsname_r_0400(void)
{
    char buf[1];
    int fd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    EXPECT_NE("ptsname_r_0400", fd, -1);
    int ret = grantpt(fd);
    EXPECT_EQ("ptsname_r_0400", ret, 0);

    ret = ptsname_r(fd, buf, sizeof(buf));
    EXPECT_EQ("ptsname_r_0400", ret, ERANGE);
    close(fd);
}

/**
 * @tc.name      : posix_openpt_0100
 * @tc.desc      : Open the ptmx file using flag O_RDWR (success)
 * @tc.level     : Level 0
 */
void posix_openpt_0100(void)
{
    int amaster, aslave;
    int ret = posix_openpt(O_RDWR);
    EXPECT_NE("posix_openpt_0100", ret, ERREXPECT);
    close(ret);
}

/**
 * @tc.name      : posix_openpt_0200
 * @tc.desc      : Open the ptmx file using flag O_RDWR | O_NOCTTY (success)
 * @tc.level     : Level 2
 */
void posix_openpt_0200(void)
{
    errno = ENOSPC;
    int ret = posix_openpt(O_RDWR | O_NOCTTY);
    EXPECT_NE("posix_openpt_0200", ret, ERREXPECT);
    close(ret);
}

int main(void)
{
    grantpt_0100();
    ptsname_r_0100();
    ptsname_r_0200();
    ptsname_r_0300();
    ptsname_r_0400();
    posix_openpt_0100();
    posix_openpt_0200();

    return t_status;
}
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

/**
 * @tc.name      : openpty_0100
 * @tc.desc      : Open the ptm file to establish a pseudo terminal, and determine
 *                 whether the terminal descriptor is successfully obtained
 * @tc.level     : Level 0
 */
void openpty_0100(void)
{
    int amaster, aslave;
    int ret = openpty(&amaster, &aslave, NULL, NULL, NULL);

    EXPECT_EQ("openpty_0100", ret, CMPFLAG);
    EXPECT_NE("openpty_0100", amaster, ERREXPECT);
    EXPECT_NE("openpty_0100", aslave, ERREXPECT);
}

/**
 * @tc.name      : openpty_0200
 * @tc.desc      : Open the ptm file to establish a pseudo terminal, and judge
 *                 whether the function successfully obtains the terminal name
 * @tc.level     : Level 0
 */
void openpty_0200(void)
{
    int amaster, aslave;
    char name[32] = {0};
    struct termios tio;
    struct winsize size;
    int ret = openpty(&amaster, &aslave, name, &tio, &size);
    EXPECT_EQ("openpty_0200", ret, CMPFLAG);
    EXPECT_NE("openpty_0200", amaster, ERREXPECT);
    EXPECT_NE("openpty_0200", aslave, ERREXPECT);
}

int main(void)
{
    openpty_0100();
    openpty_0200();
    return t_status;
}
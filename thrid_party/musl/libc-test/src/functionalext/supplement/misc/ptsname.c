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

#include <pty.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "functionalext.h"

#define PTMPATH "/dev/ptmx"

/**
 * @tc.name      : ptsname_0100
 * @tc.desc      : Open the ptm file to establish a pseudo terminal, and judge
 *                 whether the function successfully obtains the terminal name
 * @tc.level     : Level 0
 */
void ptsname_0100(void)
{
    char *mastername = NULL;

    int masterfd = open(PTMPATH, O_RDONLY);
    if (masterfd == -1) {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    mastername = ptsname(masterfd);
    EXPECT_PTRNE("ptsname_0100", mastername, NULL);

    close(masterfd);
}

/**
 * @tc.name      : ptsname_0200
 * @tc.desc      : Pass in a non-existing terminal descriptor to determine whether the function returns NULL
 * @tc.level     : Level 2
 */
void ptsname_0200(void)
{
    int aslave = -1;
    char *aslavename = NULL;

    aslavename = ptsname(aslave);
    EXPECT_PTREQ("ptsname_0200", aslavename, NULL);
}

int main(void)
{
    ptsname_0100();
    ptsname_0200();
    return t_status;
}
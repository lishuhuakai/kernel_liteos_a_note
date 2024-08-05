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

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include "test.h"

/**
 * @tc.name      : tcgetsid_0100
 * @tc.desc      : Get session ID
 * @tc.level     : Level 0
 */
void tcgetsid_0100(void)
{
    errno = 0;
    pid_t sid;

    sid = tcgetsid(STDIN_FILENO);
    if (sid == -1) {
        t_error("%s tcgetsid failed\n", __func__);
    }
    if (errno != 0) {
        t_error("%s errno is %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : tcgetsid_0200
 * @tc.desc      : Fd is not a valid file descriptor
 * @tc.level     : Level 2
 */
void tcgetsid_0200(void)
{
    errno = 0;
    pid_t sid;
    sid = tcgetsid(-1);
    if (sid != -1) {
        t_error("%s tcgetsid should failed, sid is %d", __func__, sid);
    }
    if (errno != EBADF) {
        t_error("%s errno is %d", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    tcgetsid_0100();
    tcgetsid_0200();
    return t_status;
}
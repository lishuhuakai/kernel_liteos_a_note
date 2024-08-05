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
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include "test.h"

#define BACKGROUND (0)
#define FOREGROUND (1)
#define TCGETPGRPERROR (-1)

int tcgetpgrp_handler(void)
{
    pid_t pid;
    errno = 0;
    pid = tcgetpgrp(STDIN_FILENO);
    if (pid == -1) {
        return TCGETPGRPERROR;
    } else if (pid == getpgrp()) {
        return FOREGROUND;
    } else {
        return BACKGROUND;
    }
}

/**
 * @tc.name      : tcsetpgrp_0100
 * @tc.desc      : Set terminal foreground process
 * @tc.level     : Level 0
 */
void tcsetpgrp_0100(void)
{
    int result, ret;
    pid_t spid;
    spid = tcgetsid(STDIN_FILENO);
    ret = tcgetpgrp_handler();
    if (ret != 1) {
        t_error("%s tcgetpgrp is not foreground\n", __func__);
    }
    result = tcsetpgrp(STDIN_FILENO, getpgrp());
    if (result == -1) {
        t_error("%s tcsetpgrp failed", __func__);
    }

    ret = tcgetpgrp_handler();
    result = tcsetpgrp(STDIN_FILENO, spid);
}

/**
 * @tc.name      : tcsetpgrp_0200
 * @tc.desc      : Invalid parameter input
 * @tc.level     : Level 2
 */
void tcsetpgrp_0200(void)
{
    int result;
    result = tcsetpgrp(-1, -1);
    if (result != -1) {
        t_error("%s tcsetpgrp failed, result is %d\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    tcsetpgrp_0100();
    tcsetpgrp_0200();
    return t_status;
}
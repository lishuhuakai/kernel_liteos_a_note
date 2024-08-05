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
#include <termios.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : tcsendbreak_0100
 * @tc.desc      : Send a break condition to a terminal
 * @tc.level     : Level 0
 */
void tcsendbreak_0100(void)
{
    int result = tcsendbreak(STDIN_FILENO, 0);
    if (result != 0) {
        t_error("%s tcsendbreak failed\n", __func__);
    }
}

/**
 * @tc.name      : tcsendbreak_0200
 * @tc.desc      : Fd is not a valid file descriptor
 * @tc.level     : Level 0
 */
void tcsendbreak_0200(void)
{
    errno = 0;
    int result = tcsendbreak(-1, 0);
    if (result == 0) {
        t_error("%s tcsendbreak should failed\n", __func__);
    }
    if (errno != EBADF) {
        t_error("%s errno is %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    tcsendbreak_0100();
    tcsendbreak_0200();
    return t_status;
}
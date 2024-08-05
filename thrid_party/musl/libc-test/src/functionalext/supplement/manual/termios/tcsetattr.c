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

#include "test.h"

const cc_t veof = (cc_t)0x07;

/**
 * @tc.name      : tcsetattr_0100
 * @tc.desc      : set terminal attributes
 * @tc.level     : Level 0
 */
void tcsetattr_0100(void)
{
    struct termios tio = {};
    tio.c_cc[VEOF] = veof;
    int result = tcsetattr(STDIN_FILENO, TCSAFLUSH, &tio);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = tcgetattr(STDIN_FILENO, &tio);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (tio.c_cc[VEOF] != veof) {
        t_error("%s failed: tio.c_cc[%d] = %#x\n", __func__, VEOF, tio.c_cc[VEOF]);
    }
}

/**
 * @tc.name      : tcsetattr_0200
 * @tc.desc      : set terminal attributes with invalid parameters
 * @tc.level     : Level 2
 */
void tcsetattr_0200(void)
{
    errno = 0;
    int result = tcsetattr(-1, -1, NULL);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    tcsetattr_0100();
    tcsetattr_0200();

    return t_status;
}

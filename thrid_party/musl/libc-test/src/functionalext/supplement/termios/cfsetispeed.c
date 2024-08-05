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

/**
 * @tc.name      : cfsetispeed_0100
 * @tc.desc      : Verify information that can set the input baud rate (parameter valid).
 * @tc.level     : Level 0
 */
void cfsetispeed_0100(void)
{
    struct termios t = {};
    int result = cfsetispeed(&t, B1200);
    if (result != 0) {
        t_error("%s cfsetispeed failed\n", __func__);
    }
    if (cfgetispeed(&t) != (speed_t)(B1200)) {
        t_error("%s cfsetispeed failed\n", __func__);
    }
}

/**
 * @tc.name      : cfsetispeed_0200
 * @tc.desc      : Verify that the input baud rate cannot be set (the speed parameter is invalid).
 * @tc.level     : Level 2
 */
void cfsetispeed_0200(void)
{
    struct termios t = {};
    errno = 0;
    int result = cfsetispeed(&t, 1200);
    if (result != -1) {
        t_error("%s cfsetispeed should be failed\n", __func__);
    }
    if (errno != EINVAL) {
        t_error("%s errno should be EINVAL", __func__);
    }
}

int main(int argc, char *argv[])
{
    cfsetispeed_0100();
    cfsetispeed_0200();
    return t_status;
}
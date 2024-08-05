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
 * @tc.name      : cfsetospeed_0100
 * @tc.desc      : Verify information that can set output baud rate (parameter valid).
 * @tc.level     : Level 0
 */
void cfsetospeed_0100(void)
{
    struct termios t = {};
    int result = cfsetospeed(&t, B1200);
    if (result != 0) {
        t_error("%s cfsetospeed failed\n", __func__);
    }

    speed_t ret = cfgetospeed(&t);
    if (ret != (speed_t)(B1200)) {
        t_error("%s cfgetospeed failed\n", __func__);
    }
}

/**
 * @tc.name      : cfsetospeed_0200
 * @tc.desc      : Verify that the output baud rate cannot be set (the speed parameter is invalid).
 * @tc.level     : Level 2
 */
void cfsetospeed_0200(void)
{
    struct termios t = {};
    errno = 0;

    int result = cfsetospeed(&t, 1200);
    if (result != -1) {
        t_error("%s cfsetospeed should be failed\n", __func__);
    }
    if (errno != EINVAL) {
        t_error("%s errno should be EINVAL", __func__);
    }
}

int main(int argc, char *argv[])
{
    cfsetospeed_0100();
    cfsetospeed_0200();
    return t_status;
}
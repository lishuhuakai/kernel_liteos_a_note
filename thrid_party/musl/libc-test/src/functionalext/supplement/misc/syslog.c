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
#include <syslog.h>

#include "test.h"

#define INVALID_LEVEL (-1)

/**
 * @tc.name      : syslog_0100
 * @tc.desc      : generates a log message
 * @tc.level     : Level 0
 */
void syslog_0100(void)
{
    syslog(LOG_INFO, "syslog");
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : syslog_0200
 * @tc.desc      : generates a log message with an invalid level
 * @tc.level     : Level 2
 */
void syslog_0200(void)
{
    syslog(INVALID_LEVEL, "syslog");
    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    syslog_0100();
    syslog_0200();

    return t_status;
}

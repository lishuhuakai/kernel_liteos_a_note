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
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include "test.h"

#define INVALID_LEVEL (-1)

/**
 * @tc.name      : vsyslog_0100
 * @tc.desc      : generates a log message
 * @tc.level     : Level 0
 */
void vsyslog_0100(char *s, ...)
{
    errno = 0;
    va_list ap;
    va_start(ap, s);
    vsyslog(LOG_INFO, s, ap);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
    va_end(ap);
}

/**
 * @tc.name      : vsyslog_0200
 * @tc.desc      : generates a log message with an invalid level
 * @tc.level     : Level 2
 */
void vsyslog_0200(char *s, ...)
{
    errno = 0;
    va_list ap;
    va_start(ap, s);
    vsyslog(INVALID_LEVEL, s, ap);
    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
    va_end(ap);
}

int main(int argc, char *argv[])
{
    vsyslog_0100("vsyslog: %d", 1);
    vsyslog_0200("vsyslog: %d", 1);
    return t_status;
}

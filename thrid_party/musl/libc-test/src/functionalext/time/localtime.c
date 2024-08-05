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

#include <stdlib.h>
#include <time.h>
#include "localtime_data.h"
#include "functionalext.h"

static time_t gTime = 1659177614;
static int16_t gYearBase = 1900;
static int16_t gBufferSize = 500;

extern struct tm *__localtime64 (const time_t *);

/**
 * @tc.name      : localtime_0100
 * @tc.desc      : according to different time zones, converts the time in seconds from 1970-1-1 0:00
 * to the current time system offset to local time
 * @tc.level     : Level 0
 */
void localtime_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_localtime_data) / sizeof(test_localtime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_localtime_data[i].tz);
        if (!handlerChar) {
            t_error("localtime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *localtm;
        localtm = localtime(&gTime);
        char buff[gBufferSize];
        int cnt = sprintf(buff, "%d-%d-%d %d:%d:%d wday=%d,yday=%d,isdst=%d,gmtoff=%ld,zone=%s",
            (localtm->tm_year+gYearBase), localtm->tm_mon, localtm->tm_mday, localtm->tm_hour,
            localtm->tm_min, localtm->tm_sec, localtm->tm_wday, localtm->tm_yday, localtm->tm_isdst,
            localtm->tm_gmtoff, localtm->tm_zone);
        EXPECT_TRUE("localtime_0100", cnt > 0);
        EXPECT_STREQ("localtime_0100", test_localtime_data[i].result, buff);
    }
}

/**
 * @tc.name      : localtime64_0100
 * @tc.desc      : according to different time zones, converts the time in seconds from 1970-1-1 0:00
 * to the current time system offset to local time
 * @tc.level     : Level 0
 */
void localtime64_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_localtime_data) / sizeof(test_localtime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_localtime_data[i].tz);
        if (!handlerChar) {
            t_error("localtime64_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *localtm;
        localtm = __localtime64(&gTime);
        char buff[gBufferSize];
        int cnt = sprintf(buff, "%d-%d-%d %d:%d:%d wday=%d,yday=%d,isdst=%d,gmtoff=%ld,zone=%s",
            (localtm->tm_year+gYearBase), localtm->tm_mon, localtm->tm_mday, localtm->tm_hour,
            localtm->tm_min, localtm->tm_sec, localtm->tm_wday, localtm->tm_yday, localtm->tm_isdst,
            localtm->tm_gmtoff, localtm->tm_zone);
        EXPECT_TRUE("localtime64_0100", cnt > 0);
        EXPECT_STREQ("localtime64_0100", test_localtime_data[i].result, buff);
    }
}

int main(void)
{
    localtime_0100();
    localtime64_0100();
    return t_status;
}
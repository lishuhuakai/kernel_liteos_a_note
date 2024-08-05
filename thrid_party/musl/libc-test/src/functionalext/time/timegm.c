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
#include "functionalext.h"
#include "timegm_data.h"

extern time_t __timegm_time64(struct tm *);

static time_t gTime = 1659177614;

/**
 * @tc.name      : timegm_0100
 * @tc.desc      : according to different time zones, return a timestamp from 1970-1-1 to the specified date
 * @tc.level     : Level 0
 */
void timegm_0100(void)
{
    time_t timeThis;
    for (int32_t i = 0; i < (int32_t)(sizeof(test_timegm_data) / sizeof(test_timegm_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_timegm_data[i].tz);
        if (!handlerChar) {
            t_error("timegm_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("timegm_0100", timeptr, NULL);
            return;
        }
        timeThis = timegm(timeptr);
        EXPECT_EQ("timegm_0100", test_timegm_data[i].result, timeThis);
    }
}

/**
 * @tc.name      : timegm_time64_0100
 * @tc.desc      : according to different time zones, return a timestamp from 1970-1-1 to the specified date
 * @tc.level     : Level 0
 */
void timegm_time64_0100(void)
{
    time_t timeThis;
    for (int32_t i = 0; i < (int32_t)(sizeof(test_timegm_data) / sizeof(test_timegm_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_timegm_data[i].tz);
        if (!handlerChar) {
            t_error("timegm_time64_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("timegm_time64_0100", timeptr, NULL);
            return;
        }
        timeThis = __timegm_time64(timeptr);
        EXPECT_EQ("timegm_time64_0100", test_timegm_data[i].result, timeThis);
    }
}

int main(void)
{
    timegm_0100();
    timegm_time64_0100();
    return t_status;
}
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
#include "mktime_data.h"

static time_t gTime = 1659177614;

extern time_t __mktime64 (struct tm *);

/**
 * @tc.name      : mktime_0100
 * @tc.desc      : according to different time zones, convert time to seconds in duration since 1970-1-1
 * @tc.level     : Level 0
 */
void mktime_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_mktime_data) / sizeof(test_mktime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_mktime_data[i].tz);
        if (!handlerChar) {
            t_error("mktime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("mktime_0100", timeptr, NULL);
            return;
        }
        time_t mk = mktime(timeptr);
        EXPECT_EQ("mktime_0100", mk, test_mktime_data[i].result);
    }
}

/**
 * @tc.name      : mktime64_0100
 * @tc.desc      : according to different time zones, convert time to seconds in duration since 1970-1-1
 * @tc.level     : Level 0
 */
void mktime64_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_mktime_data) / sizeof(test_mktime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_mktime_data[i].tz);
        if (!handlerChar) {
            t_error("mktime64_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("mktime64_0100", timeptr, NULL);
            return;
        }
        time_t mk = __mktime64(timeptr);
        EXPECT_EQ("mktime64_0100", mk, test_mktime_data[i].result);
    }
}

int main(void)
{
    mktime_0100();
    mktime64_0100();
    return t_status;
}
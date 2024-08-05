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
#include "difftime_data.h"
#include "functionalext.h"

extern double __difftime64 (time_t, time_t);

/**
 * @tc.name      : difftime_0100
 * @tc.desc      : according to different time zones, calculate the time difference between two moments
 * @tc.level     : Level 0
 */
void difftime_0100(void)
{
    time_t timeStart = 20000;
    time_t timeEnd = 20010;
    for (int32_t i = 0; i < (int32_t)(sizeof(test_difftime_data) / sizeof(test_difftime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_difftime_data[i].tz);
        if (!handlerChar) {
            t_error("difftime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        double returnVal;
        returnVal = difftime(timeEnd, timeStart);
        EXPECT_TRUE("difftime_0100",
            abs(test_difftime_data[i].result - returnVal) >= 0 && abs(test_difftime_data[i].result - returnVal) < 1);
    }
}


/**
 * @tc.name      : difftime64_0100
 * @tc.desc      : according to different time zones, calculate the time difference between two moments
 * @tc.level     : Level 0
 */
void difftime64_0100(void)
{
    time_t timeStart = 20000;
    time_t timeEnd = 20010;
    for (int32_t i = 0; i < (int32_t)(sizeof(test_difftime_data) / sizeof(test_difftime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_difftime_data[i].tz);
        if (!handlerChar) {
            t_error("difftime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        double returnVal;
        returnVal = __difftime64(timeEnd, timeStart);
        EXPECT_TRUE("difftime64_0100",
            abs(test_difftime_data[i].result - returnVal) >= 0 && abs(test_difftime_data[i].result - returnVal) < 1);
    }
}

int main(void)
{
    difftime_0100();
    difftime64_0100();
    return t_status;
}
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
#include "asctime_data.h"
#include "functionalext.h"

static time_t gTime = 1659177614;

/**
 * @tc.name      : asctime_0100
 * @tc.desc      : according to different time zones, take an argument representing broken-down time,
 * which is a representation separated into year, month, day, and so on
 * @tc.level     : Level 0
 */
void asctime_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(test_asctime_data) / sizeof(test_asctime_data[0])); i++) {
        const char *handlerChar = test_handle_path(test_asctime_data[i].tz);
        if (!handlerChar) {
            t_error("asctime_0100 failed: handlerChar is NULL\n");
            continue;
        }

        setenv("TZ", handlerChar, 1);
        tzset();
        struct tm *timeptr = localtime(&gTime);
        if (!timeptr) {
            EXPECT_PTRNE("asctime_0100", timeptr, NULL);
            return;
        }
        char *returnStr = asctime(timeptr);
        if (returnStr == NULL) {
            EXPECT_FALSE("asctime_0100", returnStr == NULL);
            return;
        }
        returnStr[strlen(returnStr) - 1] = 0x00;
        EXPECT_STREQ("asctime_0100", test_asctime_data[i].result, returnStr);
    }
}

int main(void)
{
    asctime_0100();
    return t_status;
}
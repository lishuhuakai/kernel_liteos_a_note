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

#define __TEST_DATA_YEAR__ 121
#define __TEST_DATA_MONTH__ 9
#define __TEST_DATA_DAY__ 3
#define __TEST_DATA_HOUR__ 9
#define __TEST_DATA_MINUTE__ 12

struct time_struct {
    const char *tz;
};

static const struct time_struct gResultData[] = {
    {"Asia/Shanghai"},
    {"Asia/Tokyo"},
    {"Europe/Moscow"},
    {"America/New_York"},
    {"Europe/Berlin"},
};

/**
 * @tc.name      : time_0100
 * @tc.desc      : according to different time zones, get time
 * @tc.level     : Level 0
 */
void time_0100(void)
{
    for (int32_t i = 0; i < (int32_t)(sizeof(gResultData) / sizeof(gResultData[0])); i++) {
        const char *tz = gResultData[i].tz;
        const char *handlerChar;
#ifdef TIME_ZONE_SUB_TAG
        char *str = strrchr(tz, TIME_ZONE_SUB_TAG);
        if (str) {
            handlerChar = ++str;
        } else {
            handlerChar = tz;
        }
#else
        handlerChar = tz;
#endif
        setenv("TZ", handlerChar, 1);
        tzset();
        system("date '2021-10-3 9:12:12' > /dev/NULL");
        time_t curClock;
        time_t t = time(&curClock);
        EXPECT_TRUE("time_0100", t > 0);
        struct tm *localtm = localtime(&curClock);
        if (!localtm) {
            EXPECT_PTRNE("time_0100", localtm, NULL);
            return;
        }
        EXPECT_EQ("time_0100", __TEST_DATA_YEAR__, localtm->tm_year);
        EXPECT_EQ("time_0100", __TEST_DATA_MONTH__, localtm->tm_mon);
        EXPECT_EQ("time_0100", __TEST_DATA_DAY__, localtm->tm_mday);
        EXPECT_EQ("time_0100", __TEST_DATA_HOUR__, localtm->tm_hour);
        EXPECT_EQ("time_0100", __TEST_DATA_MINUTE__, localtm->tm_min);
    }
}

int main(void)
{
    time_0100();
    return t_status;
}
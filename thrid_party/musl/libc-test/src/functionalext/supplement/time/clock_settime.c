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

#include <time.h>
#include <unistd.h>
#include "functionalext.h"

extern int __clock_settime64(clockid_t, const struct timespec *);

/**
 * @tc.name      : clock_settime_0100
 * @tc.desc      : Each parameter is valid, the clk parameter is CLOCK_REALTIME, which can set the system
 *                 time seconds and nanoseconds.
 * @tc.level     : Level 0
 */
void clock_settime_0100(void)
{
    struct timespec ts;
    struct tm tim = {
        .tm_year = 2022 - 1900,
        .tm_min = 16,
        .tm_hour = 18,
        .tm_mon = 6,
        .tm_mday = 6,
    };
    ts.tv_sec = mktime(&tim);
    ts.tv_nsec = 0;
    int ret = -1;
    ret = clock_settime(CLOCK_REALTIME, &ts);
    EXPECT_EQ("clock_settime_0100", ret, 0);
}

/**
 * @tc.name      : clock_settime64_0100
 * @tc.desc      : Each parameter is valid, the clk parameter is CLOCK_REALTIME, which can set the system
 *                 time seconds and nanoseconds.
 * @tc.level     : Level 0
 */
void clock_settime64_0100(void)
{
    struct timespec ts;
    struct tm tim = {
        .tm_year = 2022 - 1900,
        .tm_min = 16,
        .tm_hour = 18,
        .tm_mon = 6,
        .tm_mday = 6,
    };
    ts.tv_sec = mktime(&tim);
    ts.tv_nsec = 0;
    int ret = -1;
    ret = __clock_settime64(CLOCK_REALTIME, &ts);
    EXPECT_EQ("clock_settime64_0100", ret, 0);
}

int main(int argc, char *argv[])
{
    clock_settime_0100();
    clock_settime64_0100();
    return t_status;
}
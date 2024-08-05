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

const int SUCCESS = 0;

extern int __clock_nanosleep_time64(clockid_t, int, const struct timespec *, struct timespec *);

/**
 * @tc.name      : clock_nanosleep_0100
 * @tc.desc      : Each parameter is valid, clk is CLOCK_REALTIME, which can specify the sleep of the clock.
 * @tc.level     : Level 0
 */
void clock_nanosleep_0100(void)
{
    struct timespec ts;
    ts.tv_sec = ts.tv_nsec / 1000000000;
    ts.tv_nsec = 200000000;
    int ret = -1;
    ret = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &ts, NULL);
    EXPECT_EQ("clock_nanosleep_0100", ret, SUCCESS);
}

/**
 * @tc.name      : clock_nanosleep_0200
 * @tc.desc      : Each parameter is valid, clk is CLOCK_TAI, which can specify the sleep of the clock.
 * @tc.level     : Level 0
 */
void clock_nanosleep_0200(void)
{
    struct timespec ts;
    ts.tv_sec = ts.tv_nsec / 1000000000;
    ts.tv_nsec = 200000000;
    int ret = -1;
    ret = clock_nanosleep(CLOCK_TAI, TIMER_ABSTIME, &ts, NULL);
    EXPECT_EQ("clock_nanosleep_0200", ret, SUCCESS);
}

/**
 * @tc.name      : clock_nanosleep_0300
 * @tc.desc      : Each parameter is valid, clk is CLOCK_MONOTONIC, which can specify the sleep of the clock
 * @tc.level     : Level 0
 */
void clock_nanosleep_0300(void)
{
    struct timespec ts;
    ts.tv_sec = ts.tv_nsec / 1000000000;
    ts.tv_nsec = 200000000;
    int ret = -1;
    ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
    EXPECT_EQ("clock_nanosleep_0300", ret, SUCCESS);
}

/**
 * @tc.name      : clock_nanosleep_0400
 * @tc.desc      : Each parameter is valid, clk is CLOCK_BOOTTIME, which can specify the sleep of the clock
 * @tc.level     : Level 0
 */
void clock_nanosleep_0400(void)
{
    struct timespec ts;
    ts.tv_sec = ts.tv_nsec / 1000000000;
    ts.tv_nsec = 200000000;
    int ret = -1;
    ret = clock_nanosleep(CLOCK_BOOTTIME, TIMER_ABSTIME, &ts, NULL);
    EXPECT_EQ("clock_nanosleep_0400", ret, SUCCESS);
}

/**
 * @tc.name      : clock_nanosleep_0500
 * @tc.desc      : Each parameter is valid, clk is CLOCK_REALTIME, and flags is 0,
 *                 which can specify the sleep of the clock
 * @tc.level     : Level 1
 */
void clock_nanosleep_0500(void)
{
    struct timespec ts;
    ts.tv_sec = ts.tv_nsec / 1000000000;
    ts.tv_nsec = 200000000;
    int ret = -1;
    ret = clock_nanosleep(CLOCK_REALTIME, 0, &ts, NULL);
    EXPECT_EQ("clock_nanosleep_0500", ret, SUCCESS);
}

/**
 * @tc.name      : clock_nanosleep_0600
 * @tc.desc      : The clk parameter is invalid, clk is CLOCK_THREAD_CPUTIME_ID,
 *                 the sleep of the clock cannot be specified
 * @tc.level     : Level 2
 */
void clock_nanosleep_0600(void)
{
    struct timespec ts;
    ts.tv_sec = ts.tv_nsec / 1000000000;
    ts.tv_nsec = 200000000;
    int ret = -1;
    ret = clock_nanosleep(CLOCK_THREAD_CPUTIME_ID, TIMER_ABSTIME, &ts, NULL);
    EXPECT_EQ("clock_nanosleep_0600", ret, EINVAL);
}

/**
 * @tc.name      : clock_nanosleep_time64_0100
 * @tc.desc      : Each parameter is valid, clk is CLOCK_REALTIME, which can specify the sleep of the clock.
 * @tc.level     : Level 0
 */
void clock_nanosleep_time64_0100(void)
{
    struct timespec ts;
    ts.tv_sec = ts.tv_nsec / 1000000000;
    ts.tv_nsec = 200000000;
    int ret = -1;
    ret = __clock_nanosleep_time64(CLOCK_REALTIME, TIMER_ABSTIME, &ts, NULL);
    EXPECT_EQ("clock_nanosleep_time64_0100", ret, SUCCESS);
}

int main(int argc, char *argv[])
{
    clock_nanosleep_0100();
    clock_nanosleep_0200();
    clock_nanosleep_0300();
    clock_nanosleep_0400();
    clock_nanosleep_0500();
    clock_nanosleep_0600();
    clock_nanosleep_time64_0100();
    return t_status;
}
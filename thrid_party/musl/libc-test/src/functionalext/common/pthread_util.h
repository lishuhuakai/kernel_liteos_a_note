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

#ifndef __FUNCTIONALEXT_PTHREAD_UTIL_H__
#define __FUNCTIONALEXT_PTHREAD_UTIL_H__

#include <time.h>
#include <sys/shm.h>
#include "test.h"

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) \
        t_error("failed %d != %d \n", a, b); \
} while (0)

#define EXPECT_GE(a, b) do { \
    if ((a) < (b)) \
        t_error("failed %d < %d \n", a, b); \
} while (0)

#define EXPECT_LE(a, b) do { \
    if ((a) > (b)) \
        t_error("failed %d < %d \n", a, b); \
} while (0)

#define TEST(c, ...) ((c) || (t_error(#c " failed: " __VA_ARGS__), 0))
#define NSEC_PER_SEC 1000000000
#define NSEC_PER_100MS 100000000
#define NSEC_PER_MSEC 1000000
#define US_PER_S 1000000
#define MS_PER_S 1000
#define SLEEP_200_MS 200
#define SLEEP_100_MS 100
#define DELAY_TIME_100_MS 100
#define SLEEP_50_MS 50
#define SLEEP_20_MS 20
#define SLEEP_10_MS 10
#define CHECK_STEP_FIVE 5
#define CHECK_STEP_FOUR 4
#define CHECK_STEP_THREE 3
#define CHECK_STEP_TWO 2
#define CHECK_STEP_ONE 1

static int CARRY = 4;
static int SHMID_CHECK_STEP = 0;
static long unsigned int ONE_KB = 1024;
static int FLAG = 0666;

typedef void(*TEST_FUN)(void);

// get cur-time plus ms
static inline void GetDelayedTimeByClockid(struct timespec *ts, unsigned int ms, clockid_t clockid)
{
    const unsigned int nsecPerSec = NSEC_PER_SEC;
    unsigned int setTimeNs = ms * US_PER_S;
    struct timespec tsNow = {0};
    clock_gettime(clockid, &tsNow);
    ts->tv_sec = tsNow.tv_sec + (tsNow.tv_nsec + setTimeNs) / nsecPerSec;
    ts->tv_nsec = (tsNow.tv_nsec + setTimeNs) % nsecPerSec;
}

// calculate time difference, in ms
static inline int GetTimeDiff(struct timespec ts1, struct timespec ts2)
{
    const unsigned int nsecPerSec = NSEC_PER_SEC;
    int ms = (ts1.tv_sec - ts2.tv_sec) * nsecPerSec + (ts1.tv_nsec - ts2.tv_nsec);
    ms = ms / NSEC_PER_MSEC;
    return ms;
}

static inline void Msleep(int msec)
{
    usleep(msec * MS_PER_S);
}

#endif // __FUNCTIONALEXT_PTHREAD_UTIL_H__
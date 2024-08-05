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
#include <pthread.h>
#include "pthread_util.h"

static pthread_rwlock_t w_rwlock1;
static pthread_rwlock_t w_rwlock2;
static pthread_rwlock_t w_rwlock3;
static pthread_rwlock_t w_rwlock4;
static pthread_rwlock_t w_rwlock5;
static pthread_rwlock_t w_rwlock6;

static void *RwlockClockRealTimeOut1(void *arg)
{
    EXPECT_EQ(pthread_rwlock_wrlock(&w_rwlock1), 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock1), 0);
    return arg;
}

static void *RwlockClockRealTimeOut2(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_REALTIME);
    EXPECT_EQ(pthread_rwlock_clockwrlock(&w_rwlock1, CLOCK_REALTIME, &ts), ETIMEDOUT);
    clock_gettime(CLOCK_REALTIME, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts);
    EXPECT_GE(timeDiff, 0);
    EXPECT_LE(timeDiff, SLEEP_20_MS);
    return arg;
}

static void *RwlockClockRealTimeWait1(void *arg)
{
    EXPECT_EQ(pthread_rwlock_wrlock(&w_rwlock2), 0);
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock2), 0);
    return arg;
}

static void *RwlockClockRealTimeWait2(void *arg)
{
    struct timespec ts = {0};
    clockid_t clock_id = CLOCK_REALTIME;
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, clock_id);
    EXPECT_EQ(pthread_rwlock_clockwrlock(&w_rwlock2, clock_id, &ts), 0);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock2), 0);
    return arg;
}

static void *RwlockClockMonotonicTimeOut1(void *arg)
{
    EXPECT_EQ(pthread_rwlock_wrlock(&w_rwlock5), 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock5), 0);
    return arg;
}

static void *RwlockClockMonotonicTimeOut2(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_MONOTONIC);
    EXPECT_EQ(pthread_rwlock_clockwrlock(&w_rwlock5, CLOCK_MONOTONIC, &ts), ETIMEDOUT);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts);
    EXPECT_GE(timeDiff, 0);
    EXPECT_LE(timeDiff, SLEEP_20_MS);
    return arg;
}

static void *RwlockClockMonotonicTimeWait1(void *arg)
{
    EXPECT_EQ(pthread_rwlock_wrlock(&w_rwlock6), 0);
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock6), 0);
    return arg;
}

static void *RwlockClockMonotonicTimeWait2(void *arg)
{
    struct timespec ts = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_MONOTONIC);
    EXPECT_EQ(pthread_rwlock_clockwrlock(&w_rwlock6, CLOCK_MONOTONIC, &ts), 0);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock6), 0);
    return arg;
}

static void *RwlockMonotonicTime1(void *arg)
{
    EXPECT_EQ(pthread_rwlock_wrlock(&w_rwlock3), 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock3), 0);
    return arg;
}

static void *RwlockMonotonicTime2(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_MONOTONIC);
    EXPECT_EQ(pthread_rwlock_timedwrlock_monotonic_np(&w_rwlock3, &ts), ETIMEDOUT);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts);
    EXPECT_GE(timeDiff, 0);
    EXPECT_LE(timeDiff, SLEEP_20_MS);
    return arg;
}

static void *RwlockMonotonicTime3(void *arg)
{
    EXPECT_EQ(pthread_rwlock_wrlock(&w_rwlock4), 0);
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock4), 0);
    return arg;
}

static void *RwlockMonotonicTime4(void *arg)
{
    struct timespec ts = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_MONOTONIC);
    EXPECT_EQ(pthread_rwlock_timedwrlock_monotonic_np(&w_rwlock4, &ts), 0);
    EXPECT_EQ(pthread_rwlock_unlock(&w_rwlock4), 0);
    return arg;
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0010
 * @tc.desc      : Test whether all the interfaces to be used are normal
 * @tc.level     : Level 0
 */

void pthread_rwlock_timedwrlock_0010(void)
{
    pthread_rwlock_t w;
    struct timespec ts = {0};
    EXPECT_EQ(clock_gettime(CLOCK_MONOTONIC, &ts), 0);
    EXPECT_EQ(pthread_rwlock_init(&w, NULL), 0);
    EXPECT_EQ(pthread_rwlock_wrlock(&w), 0);
    EXPECT_EQ(pthread_rwlock_timedwrlock_monotonic_np(&w, &ts), ETIMEDOUT);
    EXPECT_EQ(pthread_rwlock_unlock(&w), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0020
 * @tc.desc      : Test the case of pthread_cond_clockwait no timeout by CLOCK_REALTIME
 * @tc.level     : Level 0
 */
void pthread_rwlock_timedwrlock_0020(void)
{
    pthread_rwlock_t w;
    struct timespec ts = {0};
    clockid_t clock_id = CLOCK_REALTIME;
    EXPECT_EQ(clock_gettime(CLOCK_REALTIME, &ts), 0);
    EXPECT_EQ(pthread_rwlock_init(&w, NULL), 0);
    EXPECT_EQ(pthread_rwlock_wrlock(&w), 0);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, clock_id);
    EXPECT_EQ(pthread_rwlock_clockwrlock(&w, clock_id, &ts), ETIMEDOUT);
    EXPECT_EQ(pthread_rwlock_unlock(&w), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0030
 * @tc.desc      : Test the case of pthread_cond_clockwait no timeout by CLOCK_REALTIME
 * @tc.level     : Level 0
 */
void pthread_rwlock_timedwrlock_0030(void)
{
    pthread_rwlock_t w;
    struct timespec ts = {0};
    clockid_t clock_id = CLOCK_MONOTONIC;
    EXPECT_EQ(clock_gettime(CLOCK_MONOTONIC, &ts), 0);
    EXPECT_EQ(pthread_rwlock_init(&w, NULL), 0);
    EXPECT_EQ(pthread_rwlock_wrlock(&w), 0);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, clock_id);
    EXPECT_EQ(pthread_rwlock_clockwrlock(&w, clock_id, &ts), ETIMEDOUT);
    EXPECT_EQ(pthread_rwlock_unlock(&w), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0040
 * @tc.desc      : Test the case of pthread_cond_clockwait no timeout by CLOCK_REALTIME
 * @tc.level     : Level 2
 */
void pthread_rwlock_timedwrlock_0040(void)
{
    pthread_rwlock_t w;
    struct timespec ts = {0};
    clockid_t clock_id = CLOCK_PROCESS_CPUTIME_ID;
    EXPECT_EQ(pthread_rwlock_init(&w, NULL), 0);
    EXPECT_EQ(pthread_rwlock_wrlock(&w), 0);
    EXPECT_EQ(pthread_rwlock_clockwrlock(&w, clock_id, &ts), EINVAL);
    EXPECT_EQ(pthread_rwlock_unlock(&w), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0050
 * @tc.desc      : Test the case of pthread_cond_clockwait no timeout by CLOCK_REALTIME
 * @tc.level     : Level 1
 */
void pthread_rwlock_timedwrlock_0050(void)
{
    pthread_t tid[2];
    EXPECT_EQ(pthread_rwlock_init(&w_rwlock2, NULL), 0);

    EXPECT_EQ(pthread_create(&tid[0], NULL, RwlockClockRealTimeWait1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid[1], NULL, RwlockClockRealTimeWait2, NULL), 0);

    EXPECT_EQ(pthread_join(tid[0], NULL), 0);
    EXPECT_EQ(pthread_join(tid[1], NULL), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w_rwlock2), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0060
 * @tc.desc      : Test the case of pthread_cond_clockwait timeout by CLOCK_REALTIME
 * @tc.level     : Level 1
 */
void pthread_rwlock_timedwrlock_0060(void)
{
    pthread_t tid[2];
    EXPECT_EQ(pthread_rwlock_init(&w_rwlock1, NULL), 0);

    EXPECT_EQ(pthread_create(&tid[0], NULL, RwlockClockRealTimeOut1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid[1], NULL, RwlockClockRealTimeOut2, NULL), 0);

    EXPECT_EQ(pthread_join(tid[0], NULL), 0);
    EXPECT_EQ(pthread_join(tid[1], NULL), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w_rwlock1), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0070
 * @tc.desc      : Test the case of pthread_cond_clockwait timeout by CLOCK_MONOTONIC
 * @tc.level     : Level 1
 */
void pthread_rwlock_timedwrlock_0070(void)
{
    pthread_t tid[2];
    EXPECT_EQ(pthread_rwlock_init(&w_rwlock5, NULL), 0);

    EXPECT_EQ(pthread_create(&tid[0], NULL, RwlockClockMonotonicTimeOut1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid[1], NULL, RwlockClockMonotonicTimeOut2, NULL), 0);

    EXPECT_EQ(pthread_join(tid[0], NULL), 0);
    EXPECT_EQ(pthread_join(tid[1], NULL), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w_rwlock5), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0080
 * @tc.desc      : Test the case of pthread_cond_clockwait timeout by CLOCK_MONOTONIC
 * @tc.level     : Level 1
 */
void pthread_rwlock_timedwrlock_0080(void)
{
    pthread_t tid[2];
    EXPECT_EQ(pthread_rwlock_init(&w_rwlock6, NULL), 0);

    EXPECT_EQ(pthread_create(&tid[0], NULL, RwlockClockMonotonicTimeWait1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid[1], NULL, RwlockClockMonotonicTimeWait2, NULL), 0);

    EXPECT_EQ(pthread_join(tid[0], NULL), 0);
    EXPECT_EQ(pthread_join(tid[1], NULL), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w_rwlock6), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0090
 * @tc.desc      : Test the case of pthread_rwlock_timedwrlock_monotonic_np no timeout
 * @tc.level     : Level 1
 */
void pthread_rwlock_timedwrlock_0090(void)
{
    pthread_t tid[2];
    EXPECT_EQ(pthread_rwlock_init(&w_rwlock3, NULL), 0);

    EXPECT_EQ(pthread_create(&tid[0], NULL, RwlockMonotonicTime1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid[1], NULL, RwlockMonotonicTime2, NULL), 0);

    EXPECT_EQ(pthread_join(tid[0], NULL), 0);
    EXPECT_EQ(pthread_join(tid[1], NULL), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w_rwlock3), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0100
 * @tc.desc      : Test the case of pthread_rwlock_timedwrlock_monotonic_np timeout
 * @tc.level     : Level 1
 */
void pthread_rwlock_timedwrlock_0100(void)
{
    pthread_t tid[2];
    EXPECT_EQ(pthread_rwlock_init(&w_rwlock4, NULL), 0);

    EXPECT_EQ(pthread_create(&tid[0], NULL, RwlockMonotonicTime3, NULL), 0);
    EXPECT_EQ(pthread_create(&tid[1], NULL, RwlockMonotonicTime4, NULL), 0);

    EXPECT_EQ(pthread_join(tid[0], NULL), 0);
    EXPECT_EQ(pthread_join(tid[1], NULL), 0);
    EXPECT_EQ(pthread_rwlock_destroy(&w_rwlock4), 0);
}

/**
 * @tc.number    : pthread_rwlock_timedwrlock_0110
 * @tc.desc      : Test the case of pthread_rwlock_timedwrlock_monotonic_np with invalid rwlock
 * @tc.level     : Level 2
 */
void pthread_rwlock_timedwrlock_0110(void)
{
    struct timespec ts = {0};
    EXPECT_EQ(pthread_rwlock_timedwrlock_monotonic_np((pthread_rwlock_t *)NULL, &ts), EINVAL);
}

TEST_FUN G_Fun_Array[] = {
    pthread_rwlock_timedwrlock_0010,
    pthread_rwlock_timedwrlock_0020,
    pthread_rwlock_timedwrlock_0030,
    pthread_rwlock_timedwrlock_0040,
    pthread_rwlock_timedwrlock_0050,
    pthread_rwlock_timedwrlock_0060,
    pthread_rwlock_timedwrlock_0070,
    pthread_rwlock_timedwrlock_0080,
    pthread_rwlock_timedwrlock_0090,
    pthread_rwlock_timedwrlock_0100,
    pthread_rwlock_timedwrlock_0110,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}
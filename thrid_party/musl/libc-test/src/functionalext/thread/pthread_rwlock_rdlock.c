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

/********************************************* Test case dividing line ***********************************************/

pthread_rwlock_t g_rwlock1;

static void *PthreadClockRdlockNoOutRealTimeW1(void *arg)
{
    TEST(pthread_rwlock_wrlock(&g_rwlock1) == 0);
    Msleep(SLEEP_50_MS);
    TEST(pthread_rwlock_unlock(&g_rwlock1) == 0);
    return arg;
}

static void *PthreadClockRdlockNoOutRealTimeR2(void *arg)
{
    struct timespec ts = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_REALTIME);
    TEST(pthread_rwlock_clockrdlock(&g_rwlock1, CLOCK_REALTIME, &ts) == 0);
    TEST(pthread_rwlock_unlock(&g_rwlock1) == 0);
    return arg;
}

/**
 * @tc.name     : pthread_rwlock_clockrdlock_0010
 * @tc.desc     : test pthread_rwlock_clockrdlock with no timeout by CLOCK_REALTIME , write - read
 * @tc.level    : Level 0
 */
static void pthread_rwlock_clockrdlock_0010(void)
{
    pthread_t tid[2];
    TEST(pthread_rwlock_init(&g_rwlock1, NULL) == 0);

    TEST(pthread_create(&tid[0], NULL, PthreadClockRdlockNoOutRealTimeW1, NULL) == 0);
    TEST(pthread_create(&tid[1], NULL, PthreadClockRdlockNoOutRealTimeR2, NULL) == 0);

    TEST(pthread_join(tid[0], NULL) == 0);
    TEST(pthread_join(tid[1], NULL) == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock1) == 0);
}

/********************************************* Test case dividing line ***********************************************/

pthread_rwlock_t g_rwlock2;

static void *PthreadClockRdlockOutRealTimeW1(void *arg)
{
    TEST(pthread_rwlock_wrlock(&g_rwlock2) == 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    TEST(pthread_rwlock_unlock(&g_rwlock2) == 0);
    return arg;
}

static void *PthreadClockRdlockOutRealTimeR2(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_REALTIME);
    TEST(pthread_rwlock_clockrdlock(&g_rwlock2, CLOCK_REALTIME, &ts) == ETIMEDOUT);
    clock_gettime(CLOCK_REALTIME, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts); // calculate time different
    TEST(timeDiff < 20);
    return arg;
}

/**
 * @tc.name     : pthread_rwlock_clockrdlock_0020
 * @tc.desc     : test pthread_rwlock_clockrdlock with timeout by CLOCK_REALTIME, write - read
 * @tc.level    : Level 0
 */
static void pthread_rwlock_clockrdlock_0020(void)
{
    pthread_t tid[2];
    TEST(pthread_rwlock_init(&g_rwlock2, NULL) == 0);

    TEST(pthread_create(&tid[0], NULL, PthreadClockRdlockOutRealTimeW1, NULL) == 0);
    TEST(pthread_create(&tid[1], NULL, PthreadClockRdlockOutRealTimeR2, NULL) == 0);

    TEST(pthread_join(tid[0], NULL) == 0);
    TEST(pthread_join(tid[1], NULL) == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock2) == 0);
}


/********************************************* Test case dividing line ***********************************************/

pthread_rwlock_t g_rwlock3;

static void *PthreadClockRdlockNoOutMonoTimeW1(void *arg)
{
    TEST(pthread_rwlock_wrlock(&g_rwlock3) == 0);
    Msleep(SLEEP_50_MS);
    TEST(pthread_rwlock_unlock(&g_rwlock3) == 0);
    return arg;
}

static void *PthreadClockRdlockNoOutMonoTimeR2(void *arg)
{
    struct timespec ts = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_rwlock_clockrdlock(&g_rwlock3, CLOCK_MONOTONIC, &ts) == 0);
    TEST(pthread_rwlock_unlock(&g_rwlock3) == 0);
    return arg;
}

/**
 * @tc.name     : pthread_rwlock_clockrdlock_0030
 * @tc.desc     : test pthread_rwlock_clockrdlock with no timeout by CLOCK_MONOTONIC , write - read
 * @tc.level    : Level 0
 */
static void pthread_rwlock_clockrdlock_0030(void)
{
    pthread_t tid[2];
    TEST(pthread_rwlock_init(&g_rwlock3, NULL) == 0);

    TEST(pthread_create(&tid[0], NULL, PthreadClockRdlockNoOutMonoTimeW1, NULL) == 0);
    TEST(pthread_create(&tid[1], NULL, PthreadClockRdlockNoOutMonoTimeR2, NULL) == 0);

    TEST(pthread_join(tid[0], NULL) == 0);
    TEST(pthread_join(tid[1], NULL) == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock3) == 0);
}

/********************************************* Test case dividing line ***********************************************/

pthread_rwlock_t g_rwlock4;

static void *PthreadClockRdlockOutMonoTimeW1(void *arg)
{
    TEST(pthread_rwlock_wrlock(&g_rwlock4) == 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    TEST(pthread_rwlock_unlock(&g_rwlock4) == 0);
    return arg;
}

static void *PthreadClockRdlockOutMonoTimeR2(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_rwlock_clockrdlock(&g_rwlock4, CLOCK_MONOTONIC, &ts) == ETIMEDOUT);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts); // calculate time different
    TEST(timeDiff < 20);
    return arg;
}

/**
 * @tc.name     : pthread_rwlock_clockrdlock_0040
 * @tc.desc     : test pthread_rwlock_clockrdlock with timeout by CLOCK_MONOTONIC, write - read
 * @tc.level    : Level 0
 */
static void pthread_rwlock_clockrdlock_0040(void)
{
    pthread_t tid[2];
    TEST(pthread_rwlock_init(&g_rwlock4, NULL) == 0);

    TEST(pthread_create(&tid[0], NULL, PthreadClockRdlockOutMonoTimeW1, NULL) == 0);
    TEST(pthread_create(&tid[1], NULL, PthreadClockRdlockOutMonoTimeR2, NULL) == 0);

    TEST(pthread_join(tid[0], NULL) == 0);
    TEST(pthread_join(tid[1], NULL) == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock4) == 0);
}

/********************************************* Test case dividing line ***********************************************/

pthread_rwlock_t g_rwlock5;

static void *PthreadTimedRdlockMonoNPNoOutW1(void *arg)
{
    TEST(pthread_rwlock_wrlock(&g_rwlock5) == 0);
    Msleep(SLEEP_50_MS);
    TEST(pthread_rwlock_unlock(&g_rwlock5) == 0);
    return arg;
}

static void *PthreadTimedRdlockMonoNPNoOutR2(void *arg)
{
    struct timespec ts = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_rwlock_timedrdlock_monotonic_np(&g_rwlock5, &ts) == 0);
    TEST(pthread_rwlock_unlock(&g_rwlock5) == 0);
    return arg;
}

/**
 * @tc.name     : pthread_rwlock_timedrdlock_monotonic_np_0010
 * @tc.desc     : test pthread_rwlock_timedrdlock_monotonic_np with no timeout by CLOCK_MONOTONIC, write - read
 * @tc.level    : Level 0
 */
static void pthread_rwlock_timedrdlock_monotonic_np_0010(void)
{
    pthread_t tid[2];
    TEST(pthread_rwlock_init(&g_rwlock5, NULL) == 0);

    TEST(pthread_create(&tid[0], NULL, PthreadTimedRdlockMonoNPNoOutW1, NULL) == 0);
    TEST(pthread_create(&tid[1], NULL, PthreadTimedRdlockMonoNPNoOutR2, NULL) == 0);

    TEST(pthread_join(tid[0], NULL) == 0);
    TEST(pthread_join(tid[1], NULL) == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock5) == 0);
}

/********************************************* Test case dividing line ***********************************************/

pthread_rwlock_t g_rwlock6;

static void *PthreadTimedRdlockMonoNPOutW1(void *arg)
{
    TEST(pthread_rwlock_wrlock(&g_rwlock6) == 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    TEST(pthread_rwlock_unlock(&g_rwlock6) == 0);
    return arg;
}

static void *PthreadTimedRdlockMonoNPOutR2(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_rwlock_timedrdlock_monotonic_np(&g_rwlock6, &ts) == ETIMEDOUT);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts); // calculate time different
    TEST(timeDiff < 20);
    return arg;
}

/**
 * @tc.name     : pthread_rwlock_timedrdlock_monotonic_np_0020
 * @tc.desc     : test pthread_rwlock_timedrdlock_monotonic_np with timeout by CLOCK_MONOTONIC, write - read
 * @tc.level    : Level 0
 */
static void pthread_rwlock_timedrdlock_monotonic_np_0020(void)
{
    pthread_t tid[2];
    TEST(pthread_rwlock_init(&g_rwlock6, NULL) == 0);

    TEST(pthread_create(&tid[0], NULL, PthreadTimedRdlockMonoNPOutW1, NULL) == 0);
    TEST(pthread_create(&tid[1], NULL, PthreadTimedRdlockMonoNPOutR2, NULL) == 0);

    TEST(pthread_join(tid[0], NULL) == 0);
    TEST(pthread_join(tid[1], NULL) == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock6) == 0);
}

/**
 * @tc.name     : pthread_rwlock_timedrdlock_monotonic_np_0030
 * @tc.desc     : test pthread_rwlock_timedrdlock_monotonic_np with invalid rwlock
 * @tc.level    : Level 2
 */
static void pthread_rwlock_timedrdlock_monotonic_np_0030(void)
{
    struct timespec ts = {0};
    EXPECT_EQ(pthread_rwlock_timedrdlock_monotonic_np((pthread_rwlock_t *)NULL, &ts), EINVAL);
}

int main(void)
{
    pthread_rwlock_clockrdlock_0010();
    pthread_rwlock_clockrdlock_0020();
    pthread_rwlock_clockrdlock_0030();
    pthread_rwlock_clockrdlock_0040();
    pthread_rwlock_timedrdlock_monotonic_np_0010();
    pthread_rwlock_timedrdlock_monotonic_np_0020();
    pthread_rwlock_timedrdlock_monotonic_np_0030();

	return t_status;
}
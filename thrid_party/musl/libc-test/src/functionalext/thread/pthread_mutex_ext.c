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

static void *PthreadClocklockOutRealTime(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    struct timespec ts = {0};
    struct timespec tsNow = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_REALTIME);
    TEST(pthread_mutex_clocklock(mtx, CLOCK_REALTIME, &ts) == ETIMEDOUT);

    clock_gettime(CLOCK_REALTIME, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts); // calculate time different
    TEST(timeDiff < 20);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_clocklock_0010
 * @tc.desc     : test pthread_mutex_clocklock whith timeout by CLOCK_REALTIME
 * @tc.level    : Level 0
 */
static void pthread_mutex_clocklock_0010(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadClocklockOutRealTime, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadClocklockNoOutRealTime(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    struct timespec ts = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_REALTIME);
    TEST(pthread_mutex_clocklock(mtx, CLOCK_REALTIME, &ts) == 0);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_clocklock_0020
 * @tc.desc     : test pthread_mutex_clocklock with no timeout by CLOCK_REALTIME
 * @tc.level    : Level 0
 */
static void pthread_mutex_clocklock_0020(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadClocklockNoOutRealTime, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadClocklockOutMonoTime(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    struct timespec ts = {0};
    struct timespec tsNow = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_mutex_clocklock(mtx, CLOCK_MONOTONIC, &ts) == ETIMEDOUT);

    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts); // calculate time different
    TEST(timeDiff < 20);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_clocklock_0030
 * @tc.desc     : test pthread_mutex_clocklock whith timeout by CLOCK_MONOTONIC
 * @tc.level    : Level 0
 */
static void pthread_mutex_clocklock_0030(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadClocklockOutMonoTime, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadClocklockNoOutMonoTime(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    struct timespec ts = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_mutex_clocklock(mtx, CLOCK_MONOTONIC, &ts) == 0);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_clocklock_0040
 * @tc.desc     : test pthread_mutex_clocklock with no timeout by CLOCK_MONOTONIC
 * @tc.level    : Level 0
 */
static void pthread_mutex_clocklock_0040(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadClocklockNoOutMonoTime, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadClocklockOutRealTime2(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    struct timespec ts = {0};
    struct timespec tsNow = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_REALTIME);
    clock_gettime(CLOCK_REALTIME, &tsNow);
    tsNow.tv_sec += 1;
    clock_settime(CLOCK_REALTIME, &tsNow);
    TEST(pthread_mutex_clocklock(mtx, CLOCK_REALTIME, &ts) == ETIMEDOUT);

    clock_gettime(CLOCK_REALTIME, &tsNow);
    tsNow.tv_sec -= 1;
    clock_settime(CLOCK_REALTIME, &tsNow);

    return arg;
}

/**
 * @tc.name     : pthread_mutex_clocklock_0050
 * @tc.desc     : test pthread_mutex_clocklock whith timeout by CLOCK_REALTIME
 * @tc.level    : Level 2
 */
static void pthread_mutex_clocklock_0050(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadClocklockOutRealTime2, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadClocklockNoOutMonoTime2(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    struct timespec ts = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_mutex_clocklock(mtx, CLOCK_MONOTONIC, &ts) == 0);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_clocklock_0060
 * @tc.desc     : test pthread_mutex_clocklock whith timeout by CLOCK_REALTIME
 * @tc.level    : Level 2
 */
static void pthread_mutex_clocklock_0060(void)
{
    struct timespec tsNow = {0};
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadClocklockNoOutMonoTime2, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    tsNow.tv_sec += 1;
    clock_settime(CLOCK_REALTIME, &tsNow);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);

    clock_gettime(CLOCK_REALTIME, &tsNow);
    tsNow.tv_sec -= 1;
    clock_settime(CLOCK_REALTIME, &tsNow);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadTimedlockMonotonicNPOut(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    struct timespec ts = {0};
    struct timespec tsNow = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_mutex_timedlock_monotonic_np(mtx, &ts) == ETIMEDOUT);

    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts); // calculate time different
    TEST(timeDiff < 20);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_timedlock_monotonic_np_0010
 * @tc.desc     : test pthread_mutex_timedlock_monotonic_np whith timeout by CLOCK_MONOTONIC
 * @tc.level    : Level 0
 */
static void pthread_mutex_timedlock_monotonic_np_0010(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadTimedlockMonotonicNPOut, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadTimedlockMonotonicNPNoOut(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    struct timespec ts = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, DELAY_TIME_100_MS, CLOCK_MONOTONIC);
    TEST(pthread_mutex_timedlock_monotonic_np(mtx, &ts) == 0);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_timedlock_monotonic_np_0020
 * @tc.desc     : test pthread_mutex_timedlock_monotonic_np with no timeout by CLOCK_MONOTONIC
 * @tc.level    : Level 0
 */
static void pthread_mutex_timedlock_monotonic_np_0020(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadTimedlockMonotonicNPNoOut, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

/********************************************* Test case dividing line ***********************************************/

/**
 * @tc.name     : pthread_mutex_timedlock_monotonic_np_0030
 * @tc.desc     : test pthread_mutex_timedlock_monotonic_np_0030 with invalid args
 * @tc.level    : Level 2
 */
static void pthread_mutex_timedlock_monotonic_np_0030(void)
{
    pthread_mutex_t *mtx = (pthread_mutex_t *)NULL;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_timedlock_monotonic_np(mtx, &ts), EINVAL);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadLockTimeoutNPOut(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    unsigned ms = DELAY_TIME_100_MS;
    struct timespec ts = {0};
    struct timespec tsNow = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, ms, CLOCK_MONOTONIC);
    TEST(pthread_mutex_lock_timeout_np(mtx, ms) == ETIMEDOUT);

    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    int timeDiff = GetTimeDiff(tsNow, ts); // calculate time different
    TEST(timeDiff < 20);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_lock_timeout_np_0010
 * @tc.desc     : test pthread_mutex_lock_timeout_np whith timeout by CLOCK_MONOTONIC
 * @tc.level    : Level 0
 */
static void pthread_mutex_lock_timeout_np_0010(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadLockTimeoutNPOut, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    Msleep(SLEEP_100_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

/********************************************* Test case dividing line ***********************************************/

static void *PthreadLockTimeoutNPNoOut(void *arg)
{
    pthread_mutex_t *mtx = (pthread_mutex_t*)arg;
    unsigned ms = DELAY_TIME_100_MS;
    struct timespec ts = {0};

    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, ms, CLOCK_MONOTONIC);
    TEST(pthread_mutex_lock_timeout_np(mtx, ms) == 0);
    return arg;
}

/**
 * @tc.name     : pthread_mutex_lock_timeout_np_0020
 * @tc.desc     : test pthread_mutex_lock_timeout_np with no timeout by CLOCK_MONOTONIC
 * @tc.level    : Level 0
 */
static void pthread_mutex_lock_timeout_np_0020(void)
{
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    pthread_t tid;
    TEST(pthread_create(&tid, NULL, PthreadLockTimeoutNPNoOut, (void*)&mtx) == 0);

    TEST(pthread_mutex_lock(&mtx) == 0);
    Msleep(SLEEP_50_MS);
    TEST(pthread_mutex_unlock(&mtx) == 0);

    TEST(pthread_join(tid, NULL) == 0);
}

int main(void)
{
    pthread_mutex_clocklock_0010();
    pthread_mutex_clocklock_0020();
    pthread_mutex_clocklock_0030();
    pthread_mutex_clocklock_0040();
    pthread_mutex_clocklock_0050();
    pthread_mutex_clocklock_0060();
    pthread_mutex_timedlock_monotonic_np_0010();
    pthread_mutex_timedlock_monotonic_np_0020();
    pthread_mutex_timedlock_monotonic_np_0030();
    pthread_mutex_lock_timeout_np_0010();
    pthread_mutex_lock_timeout_np_0020();

    return t_status;
}
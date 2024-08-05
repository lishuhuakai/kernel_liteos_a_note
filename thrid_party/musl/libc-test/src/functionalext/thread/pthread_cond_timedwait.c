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

extern int __pthread_cond_timedwait_time64(pthread_cond_t *__restrict, 
                            pthread_mutex_t *__restrict, const struct timespec *__restrict);

// pthread_cond_clockwait
static pthread_mutex_t c_mtx1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c_cond1 = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t c_mtx2 = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t c_mtx10 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c_cond2 = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t c_mtx3 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c_cond3 = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t c_mtx4 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c_cond4 = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t c_mtx5 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c_cond5 = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t c_mtx6 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t c_cond6 = PTHREAD_COND_INITIALIZER;
// pthread_cond_timedwait_monotonic_np
static pthread_mutex_t m_mtx1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t m_cond1 = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t m_mtx2 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t m_cond2 = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t m_mtx3 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t m_cond3 = PTHREAD_COND_INITIALIZER;
// pthread_cond_timeout_np
static pthread_mutex_t u_mtx1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t u_cond1 = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t u_mtx2 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t u_cond2 = PTHREAD_COND_INITIALIZER;

/**
 * @tc.number    : pthread_cond_timedwait_0010
 * @tc.desc      : Test whether the pthread_cond_timedwait is normal
 * @tc.level     : Level 0
 */
void pthread_cond_timedwait_0010(void)
{
    pthread_condattr_t a;
    EXPECT_EQ(pthread_condattr_init(&a), 0);
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    EXPECT_EQ(pthread_cond_init(&cond, &a), 0);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&mutex), 0);
    EXPECT_EQ(clock_gettime(CLOCK_REALTIME, &ts), 0);

    ts.tv_nsec += SLEEP_10_MS*MS_PER_S*MS_PER_S;
    if (ts.tv_nsec >= MS_PER_S*MS_PER_S*MS_PER_S) {
        ts.tv_nsec -= MS_PER_S*MS_PER_S*MS_PER_S;
        ts.tv_sec += 1;
    }
    EXPECT_EQ(pthread_cond_timedwait(&cond, &mutex, &ts), ETIMEDOUT);
    EXPECT_EQ(pthread_mutex_unlock(&mutex), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
    EXPECT_EQ(pthread_mutex_destroy(&mutex), 0);
}

/**
 * @tc.number    : pthread_cond_timedwait_0020
 * @tc.desc      : Test for pthread_cond_timedwait exceptions
 * @tc.level     : Level 2
 */
void pthread_cond_timedwait_0020(void)
{
    pthread_condattr_t a;
    EXPECT_EQ(pthread_condattr_init(&a), 0);
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    EXPECT_EQ(pthread_cond_init(&cond, &a), 0);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&mutex), 0);
    EXPECT_EQ(clock_gettime(CLOCK_REALTIME, &ts), 0);

    ts.tv_nsec = NSEC_PER_SEC;
    EXPECT_EQ(pthread_cond_timedwait(&cond, &mutex, &ts), EINVAL);
    EXPECT_EQ(pthread_mutex_unlock(&mutex), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
    EXPECT_EQ(pthread_mutex_destroy(&mutex), 0);
}

/**
 * @tc.number    : pthread_cond_timedwait_time64_0010
 * @tc.desc      : Test whether the pthread_cond_timedwait is normal
 * @tc.level     : Level 0
 */
void pthread_cond_timedwait_time64_0010(void)
{
    pthread_condattr_t a;
    EXPECT_EQ(pthread_condattr_init(&a), 0);
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    EXPECT_EQ(pthread_cond_init(&cond, &a), 0);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&mutex), 0);
    EXPECT_EQ(clock_gettime(CLOCK_REALTIME, &ts), 0);

    ts.tv_nsec += SLEEP_10_MS*MS_PER_S*MS_PER_S;
    if (ts.tv_nsec >= MS_PER_S*MS_PER_S*MS_PER_S) {
        ts.tv_nsec -= MS_PER_S*MS_PER_S*MS_PER_S;
        ts.tv_sec += 1;
    }
    EXPECT_EQ(__pthread_cond_timedwait_time64(&cond, &mutex, &ts), ETIMEDOUT);
    EXPECT_EQ(pthread_mutex_unlock(&mutex), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
    EXPECT_EQ(pthread_mutex_destroy(&mutex), 0);
}

/**
 * @tc.number    : pthread_cond_timedwait_monotonic_np_0010
 * @tc.desc      : Test whether the pthread_cond_timedwait_monotonic_np is normal
 * @tc.level     : Level 0
 */
void pthread_cond_timedwait_monotonic_np_0010(void)
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_cond_init(&cond, 0), 0);
    EXPECT_EQ(pthread_mutex_init(&mutex, 0), 0);

    EXPECT_EQ(clock_gettime(CLOCK_MONOTONIC, &ts), 0);
    ts.tv_nsec += SLEEP_10_MS*MS_PER_S*MS_PER_S;
    if (ts.tv_nsec >= MS_PER_S*MS_PER_S*MS_PER_S) {
        ts.tv_nsec -= MS_PER_S*MS_PER_S*MS_PER_S;
        ts.tv_sec += 1;
    }
    EXPECT_EQ(pthread_cond_timedwait_monotonic_np(&cond, &mutex, &ts), ETIMEDOUT);
    EXPECT_EQ(pthread_mutex_unlock(&mutex), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
    EXPECT_EQ(pthread_mutex_destroy(&mutex), 0);
}

/**
 * @tc.number    : pthread_cond_timedwait_monotonic_np_0020
 * @tc.desc      : Test whether the pthread_cond_timedwait_monotonic_np is invalid
 * @tc.level     : Level 2
 */
void pthread_cond_timedwait_monotonic_np_0020(void)
{
    pthread_cond_t *cond = (pthread_cond_t *)NULL;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_cond_timedwait_monotonic_np(cond, &mutex, &ts), EINVAL);
}

/**
 * @tc.number    : pthread_cond_timeout_np_0010
 * @tc.desc      : Test whether the pthread_cond_timeout_np is normal
 * @tc.level     : Level 0
 */
void pthread_cond_timeout_np_0010(void)
{
    unsigned int ms = SLEEP_100_MS;
    pthread_condattr_t a;
    EXPECT_EQ(pthread_condattr_init(&a), 0);
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    EXPECT_EQ(pthread_cond_init(&cond, &a), 0);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    EXPECT_EQ(pthread_cond_timeout_np(&cond, &mutex, ms), ETIMEDOUT);
    EXPECT_EQ(pthread_mutex_unlock(&mutex), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
    EXPECT_EQ(pthread_mutex_destroy(&mutex), 0);
}

/**
 * @tc.number    : pthread_cond_clockwait_0010
 * @tc.desc      : Test multiple cases of pthread_cond_clockwait
 * @tc.level     : Level 2
 */
void pthread_cond_clockwait_0010(void)
{
    pthread_condattr_t a;
    EXPECT_EQ(pthread_condattr_init(&a), 0);
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    EXPECT_EQ(pthread_cond_init(&cond, &a), 0);
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    struct timespec ts = {0};

    ts.tv_nsec += SLEEP_10_MS*MS_PER_S*MS_PER_S;
    if (ts.tv_nsec >= MS_PER_S*MS_PER_S*MS_PER_S) {
        ts.tv_nsec -= MS_PER_S*MS_PER_S*MS_PER_S;
        ts.tv_sec += 1;
    }
    clockid_t clock_id = CLOCK_REALTIME;
    EXPECT_EQ(pthread_cond_clockwait(&cond, &mutex, clock_id, &ts), ETIMEDOUT);
    clock_id = CLOCK_MONOTONIC;
    EXPECT_EQ(pthread_cond_clockwait(&cond, &mutex, clock_id, &ts), ETIMEDOUT);
    clock_id = CLOCK_PROCESS_CPUTIME_ID;
    EXPECT_EQ(pthread_cond_clockwait(&cond, &mutex, clock_id, &ts), EINVAL);
    EXPECT_EQ(pthread_mutex_unlock(&mutex), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
    EXPECT_EQ(pthread_mutex_destroy(&mutex), 0);
}

/**
 * @tc.number    : pthread_cond_timedwait_Time_0010
 * @tc.desc      : Whether the interface is normal when the time is not set
 * @tc.level     : Level 0
 */
void pthread_cond_timedwait_Time_0010(void)
{
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    struct timespec ts;
    EXPECT_EQ(pthread_cond_init(&cond, 0), 0);
    EXPECT_EQ(pthread_mutex_init(&mutex, 0), 0);

    EXPECT_EQ(pthread_cond_timedwait_monotonic_np(&cond, &mutex, &ts), ETIMEDOUT);

    EXPECT_EQ(pthread_mutex_unlock(&mutex), 0);
    EXPECT_EQ(pthread_cond_destroy(&cond), 0);
    EXPECT_EQ(pthread_mutex_destroy(&mutex), 0);
}

static void *ClockWaitTimedwait1(void *arg)
{
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_mutex_lock(&c_mtx1), 0);
    EXPECT_EQ(pthread_mutex_unlock(&c_mtx1), 0);
    EXPECT_EQ(pthread_cond_signal(&c_cond1), 0);
    return arg;
}

static void *ClockWaitTimedwait2(void *arg)
{
    const unsigned int nsecPerSec = NSEC_PER_SEC;
    const unsigned int nsecPer100Ms = NSEC_PER_100MS;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&c_mtx1), 0);

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec = ts.tv_sec + (ts.tv_nsec + nsecPer100Ms) / nsecPerSec;
    ts.tv_nsec = (ts.tv_nsec + nsecPer100Ms) % nsecPerSec;

    clockid_t clock_id = CLOCK_REALTIME;
    EXPECT_EQ(pthread_cond_clockwait(&c_cond1, &c_mtx1, clock_id, &ts), 0);
    EXPECT_EQ(pthread_mutex_unlock(&c_mtx1), 0);
    return arg;
}

/**
 * @tc.number    : clockwait_timedwait_0010
 * @tc.desc      : Test the case of pthread_cond_clockwait timewait by CLOCK_REALTIME
 * @tc.level     : Level 1
 */
void clockwait_timedwait_0010(void)
{
    pthread_t tid1;
    pthread_t tid2;
    EXPECT_EQ(pthread_create(&tid1, NULL, ClockWaitTimedwait1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid2, NULL, ClockWaitTimedwait2, NULL), 0);
    Msleep(SLEEP_100_MS);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    EXPECT_EQ(pthread_cond_destroy(&c_cond1), 0);
    EXPECT_EQ(pthread_mutex_destroy(&c_mtx1), 0);
}

static void *ClockWaitTimeOut(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    EXPECT_EQ(pthread_mutex_lock(&c_mtx2), 0);

    clockid_t clock_id = CLOCK_REALTIME;
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, clock_id);
    EXPECT_EQ(pthread_cond_clockwait(&c_cond2, &c_mtx2, clock_id, &ts), ETIMEDOUT);
    clock_gettime(CLOCK_REALTIME, &tsNow);

    int timeDiff = GetTimeDiff(tsNow, ts);
    EXPECT_GE(timeDiff, 0);
    EXPECT_LE(timeDiff, SLEEP_20_MS);

    EXPECT_EQ(pthread_mutex_unlock(&c_mtx2), 0);
    return arg;
}

/**
 * @tc.number    : clockwait_timedwait_0020
 * @tc.desc      : Test the case of pthread_cond_clockwait timeout by CLOCK_REALTIME
 * @tc.level     : Level 1
 */
void clockwait_timedwait_0020(void)
{
    pthread_t tid;
    EXPECT_EQ(pthread_create(&tid, NULL, ClockWaitTimeOut, NULL), 0);
    Msleep(SLEEP_200_MS);
    pthread_join(tid, NULL);
    EXPECT_EQ(pthread_cond_destroy(&c_cond2), 0);
    EXPECT_EQ(pthread_mutex_destroy(&c_mtx2), 0);
}

static void *ClockWaitTimedwait3(void *arg)
{
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_mutex_lock(&c_mtx3), 0);
    EXPECT_EQ(pthread_mutex_unlock(&c_mtx3), 0);
    EXPECT_EQ(pthread_cond_signal(&c_cond3), 0);
    return arg;
}

static void *ClockWaitTimedwait4(void *arg)
{
    const unsigned int nsecPerSec = NSEC_PER_SEC;
    const unsigned int nsecPer100Ms = NSEC_PER_100MS;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&c_mtx3), 0);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    ts.tv_sec = ts.tv_sec + (ts.tv_nsec + nsecPer100Ms) / nsecPerSec;
    ts.tv_nsec = (ts.tv_nsec + nsecPer100Ms) % nsecPerSec;

    clockid_t clock_id = CLOCK_MONOTONIC;
    EXPECT_EQ(pthread_cond_clockwait(&c_cond3, &c_mtx3, clock_id, &ts), 0);
    EXPECT_EQ(pthread_mutex_unlock(&c_mtx3), 0);
    return arg;
}

/**
 * @tc.number    : clockwait_timedwait_0030
 * @tc.desc      : Test the case of pthread_cond_clockwait timewait by CLOCK_MONOTONIC
 * @tc.level     : Level 1
 */
void clockwait_timedwait_0030(void)
{
    pthread_t tid1;
    pthread_t tid2;
    EXPECT_EQ(pthread_create(&tid1, NULL, ClockWaitTimedwait3, NULL), 0);
    EXPECT_EQ(pthread_create(&tid2, NULL, ClockWaitTimedwait4, NULL), 0);
    Msleep(SLEEP_100_MS);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    EXPECT_EQ(pthread_cond_destroy(&c_cond3), 0);
    EXPECT_EQ(pthread_mutex_destroy(&c_mtx3), 0);
}

static void *ClockWaitTimeOut2(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    EXPECT_EQ(pthread_mutex_lock(&c_mtx10), 0);

    clockid_t clock_id = CLOCK_MONOTONIC;
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, clock_id);
    EXPECT_EQ(pthread_cond_clockwait(&c_cond2, &c_mtx10, clock_id, &ts), ETIMEDOUT);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);

    int timeDiff = GetTimeDiff(tsNow, ts);
    EXPECT_GE(timeDiff, 0);
    EXPECT_LE(timeDiff, SLEEP_20_MS);

    EXPECT_EQ(pthread_mutex_unlock(&c_mtx10), 0);
    return arg;
}

/**
 * @tc.number    : clockwait_timedwait_0040
 * @tc.desc      : Test the case of pthread_cond_clockwait timeout by CLOCK_MONOTONIC
 * @tc.level     : Level 1
 */
void clockwait_timedwait_0040(void)
{
    pthread_t tid;
    EXPECT_EQ(pthread_create(&tid, NULL, ClockWaitTimeOut2, NULL), 0);
    Msleep(SLEEP_200_MS);
    pthread_join(tid, NULL);
    EXPECT_EQ(pthread_cond_destroy(&c_cond2), 0);
    EXPECT_EQ(pthread_mutex_destroy(&c_mtx10), 0);
}

static void *ClockWaitTimeMismatch(void *arg)
{
    const unsigned int nsecPerSec = NSEC_PER_SEC;
    const unsigned int nsecPer100Ms = NSEC_PER_100MS;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&c_mtx4), 0);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    ts.tv_sec = ts.tv_sec + (ts.tv_nsec + nsecPer100Ms) / nsecPerSec;
    ts.tv_nsec = (ts.tv_nsec + nsecPer100Ms) % nsecPerSec;

    clockid_t clock_id = CLOCK_REALTIME;
    EXPECT_EQ(pthread_cond_clockwait(&c_cond4, &c_mtx4, clock_id, &ts), ETIMEDOUT);

    EXPECT_EQ(pthread_mutex_unlock(&c_mtx4), 0);
    return arg;
}

static void *ClockWaitTimeMismatch1(void *arg)
{
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_mutex_lock(&c_mtx5), 0);
    EXPECT_EQ(pthread_mutex_unlock(&c_mtx5), 0);
    EXPECT_EQ(pthread_cond_signal(&c_cond5), 0);
    return arg;
}

static void *ClockWaitTimeMismatch2(void *arg)
{
    const unsigned int nsecPerSec = NSEC_PER_SEC;
    const unsigned int nsecPer100Ms = NSEC_PER_100MS;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&c_mtx5), 0);

    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec = ts.tv_sec + (ts.tv_nsec + nsecPer100Ms) / nsecPerSec;
    ts.tv_nsec = (ts.tv_nsec + nsecPer100Ms) % nsecPerSec;

    clockid_t clock_id = CLOCK_REALTIME;
    EXPECT_EQ(pthread_cond_clockwait(&c_cond5, &c_mtx5, clock_id, &ts), 0);

    EXPECT_EQ(pthread_mutex_unlock(&c_mtx5), 0);
    return arg;
}

/**
 * @tc.number    : clockwait_timedwait_0050
 * @tc.desc      : Test the case of pthread_cond_clockwait time mismatch
 * @tc.level     : Level 2
 */
void clockwait_timedwait_0050(void)
{
    pthread_t tid;
    pthread_t tid1;
    pthread_t tid2;
    EXPECT_EQ(pthread_create(&tid1, NULL, ClockWaitTimeMismatch1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid2, NULL, ClockWaitTimeMismatch2, NULL), 0);
    EXPECT_EQ(pthread_create(&tid, NULL, ClockWaitTimeMismatch, NULL), 0);
    Msleep(SLEEP_200_MS);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid, NULL);
    EXPECT_EQ(pthread_cond_destroy(&c_cond4), 0);
    EXPECT_EQ(pthread_mutex_destroy(&c_mtx4), 0);
    EXPECT_EQ(pthread_cond_destroy(&c_cond5), 0);
    EXPECT_EQ(pthread_mutex_destroy(&c_mtx5), 0);
}

static void *ClockWaitTimeMismatch3(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    EXPECT_EQ(pthread_mutex_lock(&c_mtx6), 0);
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_REALTIME);
    clock_gettime(CLOCK_REALTIME, &tsNow);
    tsNow.tv_sec += 1;
    EXPECT_EQ(clock_settime(CLOCK_REALTIME, &tsNow), 0);
    EXPECT_EQ(pthread_cond_clockwait(&c_cond6, &c_mtx6, CLOCK_REALTIME, &ts), ETIMEDOUT);

    clock_gettime(CLOCK_REALTIME, &tsNow);
    tsNow.tv_sec -= 1;
    clock_settime(CLOCK_REALTIME, &tsNow);
    EXPECT_EQ(pthread_mutex_unlock(&c_mtx6), 0);

    return arg;
}

/**
 * @tc.number    : clockwait_timedwait_0060
 * @tc.desc      : A case for testing the CLOCK_REALTIME feature of pthread_cond_clockwait.
 * @tc.level     : Level 1
 */
void clockwait_timedwait_0060(void)
{
    pthread_t tid;
    EXPECT_EQ(pthread_create(&tid, NULL, ClockWaitTimeMismatch3, NULL), 0);
    Msleep(SLEEP_200_MS);
    pthread_join(tid, NULL);
    EXPECT_EQ(pthread_cond_destroy(&c_cond6), 0);
    EXPECT_EQ(pthread_mutex_destroy(&c_mtx6), 0);
}

static void *ClockWaitTimeMismatch4(void *arg)
{
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_mutex_lock(&c_mtx6), 0);
    EXPECT_EQ(pthread_mutex_unlock(&c_mtx6), 0);
    EXPECT_EQ(pthread_cond_signal(&c_cond6), 0);
    return arg;
}

static void *ClockWaitTimeMismatch5(void *arg)
{
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&c_mtx6), 0);
    Msleep(SLEEP_20_MS);
    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_MONOTONIC);
    EXPECT_EQ(pthread_cond_clockwait(&c_cond6, &c_mtx6, CLOCK_MONOTONIC, &ts), 0);
    EXPECT_EQ(pthread_mutex_unlock(&c_mtx6), 0);
    return arg;
}

/**
 * @tc.number    : clockwait_timedwait_0070
 * @tc.desc      : A case for testing the CLOCK_MONOTONIC feature of pthread_cond_clockwait.
 * @tc.level     : Level 1
 */
void clockwait_timedwait_0070(void)
{
    struct timespec tsNow = {0};
    pthread_t tid1;
    pthread_t tid2;

    EXPECT_EQ(pthread_create(&tid1, NULL, ClockWaitTimeMismatch4, NULL), 0);
    EXPECT_EQ(pthread_create(&tid2, NULL, ClockWaitTimeMismatch5, NULL), 0);
    Msleep(SLEEP_100_MS);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);
    tsNow.tv_sec += 1;
    EXPECT_EQ(clock_settime(CLOCK_REALTIME, &tsNow), 0);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    EXPECT_EQ(pthread_cond_destroy(&c_cond6), 0);
    EXPECT_EQ(pthread_mutex_destroy(&c_mtx6), 0);
}

static void *PthreadCondMonotonicTimeWait1(void *arg)
{
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_mutex_lock(&m_mtx1), 0);
    EXPECT_EQ(pthread_mutex_unlock(&m_mtx1), 0);
    EXPECT_EQ(pthread_cond_signal(&m_cond1), 0);
    return arg;
}

static void *PthreadCondMonotonicTimeWait2(void *arg)
{
    const unsigned int nsecPerSec = NSEC_PER_SEC;
    const unsigned int nsecPer100Ms = NSEC_PER_100MS;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&m_mtx1), 0);

    clock_gettime(CLOCK_MONOTONIC, &ts);
    ts.tv_sec = ts.tv_sec + (ts.tv_nsec + nsecPer100Ms) / nsecPerSec;
    ts.tv_nsec = (ts.tv_nsec + nsecPer100Ms) % nsecPerSec;

    EXPECT_EQ(pthread_cond_timedwait_monotonic_np(&m_cond1, &m_mtx1, &ts), 0);
    EXPECT_EQ(pthread_mutex_unlock(&m_mtx1), 0);
    return arg;
}

/**
 * @tc.number    : monotonic_timewait_0010
 * @tc.desc      : Test the case of pthread_cond_timedwait_monotonic_np timewait
 * @tc.level     : Level 1
 */
void monotonic_timewait_0010(void)
{
    pthread_t tid1;
    pthread_t tid2;

    EXPECT_EQ(pthread_create(&tid1, NULL, PthreadCondMonotonicTimeWait1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid2, NULL, PthreadCondMonotonicTimeWait2, NULL), 0);

    Msleep(SLEEP_100_MS);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    EXPECT_EQ(pthread_cond_destroy(&m_cond1), 0);
    EXPECT_EQ(pthread_mutex_destroy(&m_mtx1), 0);
}

static void *PthreadCondMonotonicTimeOut(void *arg)
{
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    EXPECT_EQ(pthread_mutex_lock(&m_mtx2), 0);

    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_MONOTONIC);
    EXPECT_EQ(pthread_cond_timedwait_monotonic_np(&m_cond2, &m_mtx2, &ts), ETIMEDOUT);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);

    int timeDiff = GetTimeDiff(tsNow, ts);
    EXPECT_GE(timeDiff, 0);
    EXPECT_LE(timeDiff, SLEEP_20_MS);

    EXPECT_EQ(pthread_mutex_unlock(&m_mtx2), 0);
    return arg;
}

/**
 * @tc.number    : monotonic_timewait_0020
 * @tc.desc      : Test the case of pthread_cond_timedwait_monotonic_np timeout
 * @tc.level     : Level 1
 */
void monotonic_timewait_0020(void)
{
    pthread_t tid;
    EXPECT_EQ(pthread_create(&tid, NULL, PthreadCondMonotonicTimeOut, NULL), 0);
    Msleep(SLEEP_200_MS);
    pthread_join(tid, NULL);
    EXPECT_EQ(pthread_cond_destroy(&m_cond2), 0);
    EXPECT_EQ(pthread_mutex_destroy(&m_mtx2), 0);
}

static void *PthreadCondMonotonicTimeEinval(void *arg)
{
    const long einvalNsec = NSEC_PER_SEC;
    struct timespec ts = {0};
    EXPECT_EQ(pthread_mutex_lock(&m_mtx3), 0);

    ts.tv_sec = 1;
    ts.tv_nsec = einvalNsec;
    EXPECT_EQ(pthread_cond_timedwait_monotonic_np(&m_cond3, &m_mtx3, &ts), EINVAL);

    EXPECT_EQ(pthread_mutex_unlock(&m_mtx3), 0);
    return arg;
}

/**
 * @tc.number    : monotonic_timewait_0030
 * @tc.desc      : Test the case of pthread_cond_timedwait_monotonic_np EINVAL
 * @tc.level     : Level 2
 */
void monotonic_timewait_0030(void)
{
    pthread_t tid;

    EXPECT_EQ(pthread_create(&tid, NULL, PthreadCondMonotonicTimeEinval, NULL), 0);

    Msleep(SLEEP_200_MS);
    pthread_join(tid, NULL);
    EXPECT_EQ(pthread_cond_destroy(&m_cond3), 0);
    EXPECT_EQ(pthread_mutex_destroy(&m_mtx3), 0);
}

static void *PthreadCondUnsignedTimeWait1(void *arg)
{
    Msleep(SLEEP_50_MS);
    EXPECT_EQ(pthread_mutex_lock(&u_mtx1), 0);
    EXPECT_EQ(pthread_mutex_unlock(&u_mtx1), 0);
    EXPECT_EQ(pthread_cond_signal(&u_cond1), 0);
    return arg;
}

static void *PthreadCondUnsignedTimeWait2(void *arg)
{
    unsigned int ms = SLEEP_100_MS;
    struct timespec ts = {0};

    GetDelayedTimeByClockid(&ts, ms, CLOCK_MONOTONIC);
    EXPECT_EQ(pthread_mutex_lock(&u_mtx1), 0);
    EXPECT_EQ(pthread_cond_timeout_np(&u_cond1, &u_mtx1, ms), 0);
    EXPECT_EQ(pthread_mutex_unlock(&u_mtx1), 0);
    return arg;
}

/**
 * @tc.number    : timeoutnp_timewait_0010
 * @tc.desc      : Test the case of pthread_cond_timeout_np timewait
 * @tc.level     : Level 1
 */
void timeoutnp_timewait_0010(void)
{
    pthread_t tid1;
    pthread_t tid2;

    EXPECT_EQ(pthread_create(&tid1, NULL, PthreadCondUnsignedTimeWait1, NULL), 0);
    EXPECT_EQ(pthread_create(&tid2, NULL, PthreadCondUnsignedTimeWait2, NULL), 0);

    Msleep(SLEEP_100_MS);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    EXPECT_EQ(pthread_cond_destroy(&u_cond1), 0);
    EXPECT_EQ(pthread_mutex_destroy(&u_mtx1), 0);
}

static void *PthreadCondUnsignedTimeOut(void *arg)
{
    unsigned int ms = SLEEP_100_MS;
    struct timespec ts = {0};
    struct timespec tsNow = {0};
    EXPECT_EQ(pthread_mutex_lock(&u_mtx2), 0);

    GetDelayedTimeByClockid(&ts, SLEEP_100_MS, CLOCK_MONOTONIC);
    EXPECT_EQ(pthread_cond_timeout_np(&u_cond2, &u_mtx2, ms), ETIMEDOUT);
    clock_gettime(CLOCK_MONOTONIC, &tsNow);

    int timeDiff = GetTimeDiff(tsNow, ts);
    EXPECT_GE(timeDiff, 0);
    EXPECT_LE(timeDiff, SLEEP_20_MS);

    EXPECT_EQ(pthread_mutex_unlock(&u_mtx2), 0);
    return arg;
}

/**
 * @tc.number    : timeoutnp_timewait_0020
 * @tc.desc      : Test the case of pthread_cond_timeout_np timeout
 * @tc.level     : Level 1
 */
void timeoutnp_timewait_0020(void)
{
    pthread_t tid;
    EXPECT_EQ(pthread_create(&tid, NULL, PthreadCondUnsignedTimeOut, NULL), 0);
    Msleep(SLEEP_200_MS);
    pthread_join(tid, NULL);
    EXPECT_EQ(pthread_cond_destroy(&u_cond2), 0);
    EXPECT_EQ(pthread_mutex_destroy(&u_mtx2), 0);
}

TEST_FUN G_Fun_Array[] = {
    pthread_cond_timedwait_0010,
    pthread_cond_timedwait_0020,
    pthread_cond_timedwait_time64_0010,
    pthread_cond_timedwait_monotonic_np_0010,
    pthread_cond_timedwait_monotonic_np_0020,
    pthread_cond_timeout_np_0010,
    pthread_cond_clockwait_0010,
    pthread_cond_timedwait_Time_0010,
    clockwait_timedwait_0010,
    clockwait_timedwait_0020,
    clockwait_timedwait_0030,
    clockwait_timedwait_0040,
    clockwait_timedwait_0050,
    monotonic_timewait_0010,
    monotonic_timewait_0020,
    monotonic_timewait_0030,
    timeoutnp_timewait_0010,
    timeoutnp_timewait_0020,
    clockwait_timedwait_0060,
    clockwait_timedwait_0070,
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos)     {
        G_Fun_Array[pos]();
    }
    return t_status;
}

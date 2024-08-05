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

#include <pthread.h>
#include <stdlib.h>
#include "functionalext.h"

#define TEST_MUTEX_ATTR 15

extern int __pthread_mutex_timedlock_time64(pthread_mutex_t *__restrict, const struct timespec *__restrict);

/**
 * @tc.name      : pthread_mutex_timedlock_0100
 * @tc.desc      : Provide correct parameters, test timeout and lock
 * @tc.level     : Level 0
 */
void pthread_mutex_timedlock_0100(void)
{
    struct timespec ts = {.tv_nsec = 0, .tv_sec = 1};
    pthread_mutexattr_t mutex_attr;
    pthread_mutex_t mutex;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, TEST_MUTEX_ATTR);
    pthread_mutex_init(&mutex, &mutex_attr);

    int rev = pthread_mutex_timedlock(&mutex, &ts);
    EXPECT_EQ("pthread_mutex_timedlock_0100", rev, 0);

    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);
}

/**
 * @tc.name      : pthread_mutex_timedlock_0200
 * @tc.desc      : The lock attribute is PTHREAD_MUTEX_ERRORCHECK, and the test timeout is locked
 * @tc.level     : Level 0
 */
void pthread_mutex_timedlock_0200(void)
{
    struct timespec ts = {.tv_nsec = 0, .tv_sec = 1};
    pthread_mutexattr_t mutex_attr;
    pthread_mutex_t mutex;

    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mutex, &mutex_attr);
    int rev = pthread_mutex_timedlock(&mutex, &ts);
    EXPECT_EQ("pthread_mutex_timedlock_0200", rev, 0);

    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);
}

/**
 * @tc.name      : pthread_mutex_timedlock_0300
 * @tc.desc      : In the locked state, the test times out and locks
 * @tc.level     : Level 2
 */
void pthread_mutex_timedlock_0300(void)
{
    struct timespec ts = {.tv_nsec = 0, .tv_sec = 1};
    pthread_mutexattr_t mutex_attr;
    pthread_mutex_t mutex;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, TEST_MUTEX_ATTR);
    pthread_mutex_init(&mutex, &mutex_attr);
    int rev = pthread_mutex_lock(&mutex);
    EXPECT_EQ("pthread_mutex_timedlock_0300", rev, 0);
    rev = pthread_mutex_timedlock(&mutex, &ts);
    EXPECT_EQ("pthread_mutex_timedlock_0300", rev, ETIMEDOUT);

    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);
}

/**
 * @tc.name      : pthread_mutex_timedlock_time64_0100
 * @tc.desc      : Provide correct parameters, test timeout and lock
 * @tc.level     : Level 0
 */
void pthread_mutex_timedlock_time64_0100(void)
{
    struct timespec ts = {.tv_nsec = 0, .tv_sec = 1};
    pthread_mutexattr_t mutex_attr;
    pthread_mutex_t mutex;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, TEST_MUTEX_ATTR);
    pthread_mutex_init(&mutex, &mutex_attr);

    int rev = __pthread_mutex_timedlock_time64(&mutex, &ts);
    EXPECT_EQ("pthread_mutex_timedlock_time64_0100", rev, 0);

    pthread_mutex_unlock(&mutex);
    pthread_mutex_destroy(&mutex);
    pthread_mutexattr_destroy(&mutex_attr);
}

int main(void)
{
    pthread_mutex_timedlock_0100();
    pthread_mutex_timedlock_0200();
    pthread_mutex_timedlock_0300();
    pthread_mutex_timedlock_time64_0100();
    return t_status;
}
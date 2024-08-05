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

extern int __pthread_rwlock_timedwrlock_time64(pthread_rwlock_t *__restrict, const struct timespec *__restrict);

/**
 * @tc.name      : pthread_rwlock_timedwrlock_0100
 * @tc.desc      : Provide correct parameters, test timeout and rwlock
 * @tc.level     : Level 0
 */
void pthread_rwlock_timedwrlock_0100(void)
{
    struct timespec ts = {.tv_nsec = 0, .tv_sec = 0};
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *tmp = localtime(&ts.tv_sec);
    int rev = -1;
    if (tmp) {
        ts.tv_sec += 1;
        rev = pthread_rwlock_timedwrlock(&rwlock, &ts);
    }
    EXPECT_EQ("pthread_rwlock_timedwrlock_0100", rev, 0);
    if (rev == 0) {
        pthread_rwlock_unlock(&rwlock);
    }
    pthread_rwlock_destroy(&rwlock);
}

/**
 * @tc.name      : pthread_rwlock_timedwrlock_0200
 * @tc.desc      : In the locked (read lock) state, the test times out and rwlock
 * @tc.level     : Level 2
 */
void pthread_rwlock_timedwrlock_0200(void)
{
    struct timespec ts = {.tv_nsec = 0, .tv_sec = 0};
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);
    int rev = pthread_rwlock_timedrdlock(&rwlock, NULL);
    EXPECT_EQ("pthread_rwlock_timedwrlock_0200", rev, 0);
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *tmp = localtime(&ts.tv_sec);
    rev = -1;
    if (tmp) {
        ts.tv_sec += 1;
        rev = pthread_rwlock_timedwrlock(&rwlock, &ts);
    }
    EXPECT_EQ("pthread_rwlock_timedwrlock_0200", rev, ETIMEDOUT);
    pthread_rwlock_unlock(&rwlock);
    pthread_rwlock_destroy(&rwlock);
}

/**
 * @tc.name      : pthread_rwlock_timedwrlock_0300
 * @tc.desc      : In the locked (write lock) state, the test times out and rwlock
 * @tc.level     : Level 2
 */
void pthread_rwlock_timedwrlock_0300(void)
{
    struct timespec ts = {.tv_nsec = 0, .tv_sec = 0};
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);
    int rev = pthread_rwlock_timedwrlock(&rwlock, NULL);
    EXPECT_EQ("pthread_rwlock_timedwrlock_0300", rev, 0);
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *tmp = localtime(&ts.tv_sec);
    rev = -1;
    if (tmp) {
        ts.tv_sec += 1;
        rev = pthread_rwlock_timedwrlock(&rwlock, &ts);
    }
    EXPECT_EQ("pthread_rwlock_timedwrlock_0300", rev, ETIMEDOUT);
    pthread_rwlock_unlock(&rwlock);
    pthread_rwlock_destroy(&rwlock);
}

/**
 * @tc.name      : pthread_rwlock_timedwrlock_time64_0100
 * @tc.desc      : Provide correct parameters, test timeout and rwlock
 * @tc.level     : Level 0
 */
void pthread_rwlock_timedwrlock_time64_0100(void)
{
    struct timespec ts = {.tv_nsec = 0, .tv_sec = 0};
    pthread_rwlock_t rwlock;
    pthread_rwlock_init(&rwlock, NULL);
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *tmp = localtime(&ts.tv_sec);
    int rev = -1;
    if (tmp) {
        ts.tv_sec += 1;
        rev = __pthread_rwlock_timedwrlock_time64(&rwlock, &ts);
    }
    EXPECT_EQ("pthread_rwlock_timedwrlock_time64_0100", rev, 0);
    if (rev == 0) {
        pthread_rwlock_unlock(&rwlock);
    }
    pthread_rwlock_destroy(&rwlock);
}

int main(void)
{
    pthread_rwlock_timedwrlock_0100();
    pthread_rwlock_timedwrlock_0200();
    pthread_rwlock_timedwrlock_0300();
    pthread_rwlock_timedwrlock_time64_0100();
    return t_status;
}
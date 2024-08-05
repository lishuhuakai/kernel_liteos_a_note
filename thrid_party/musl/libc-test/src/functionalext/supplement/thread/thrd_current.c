/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 */
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

#include <signal.h>
#include <stdlib.h>
#include <threads.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include "test.h"

#define LOCK_SUCCESS (0)
#define LOCK_TIMEOUT (-1)
#define LOCK_WAIT_MAX_TIME_COUNT (50)
#define NSEC_TO_MS (1000000)
#define TRYLOCK_FAIL (-2)

static thrd_t thr;
static int count = 0;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

int TrylockWithTimeout(pthread_mutex_t *mutex)
{
    int rc;

    int sleepCount = 0;

    do {
        rc = pthread_mutex_trylock(mutex);
        if (rc == 0) {
            return LOCK_SUCCESS;
        } else if (rc != EBUSY) {
            perror("pthread_mutex_trylock");
            return TRYLOCK_FAIL;
        }

        struct timespec req = { .tv_sec = 0, .tv_nsec = NSEC_TO_MS * 10 }; // 等待10毫秒
        nanosleep(&req, NULL);
        
        sleepCount++;
        if (sleepCount >= LOCK_WAIT_MAX_TIME_COUNT) {
            return LOCK_TIMEOUT;
        }
    } while (1);
}

int threadfuncA(void *arg)
{
    count++;
    thrd_t id = thrd_current();
    if (!(thrd_equal(id, thr))) {
        t_error("%s thrd_current failed", __func__);
    }

    thrd_exit(thrd_success);
}

int threadfuncB(void *arg)
{
    int result = TrylockWithTimeout(&g_mutex);
    if (result == LOCK_SUCCESS) {
        count++;
        pthread_mutex_unlock(&g_mutex);
    } else if (result == LOCK_TIMEOUT) {
        t_error("Lock acquisition timed out.\n");
    } else {
        t_error("Error pthread_mutex_trylock.\n");
    }

    thrd_exit(thrd_success);
}

/**
 * @tc.name      : thrd_current_0100
 * @tc.desc      : Returns the identifier of the calling thread
 * @tc.level     : Level 0
 */
void thrd_current_0100(void)
{
    int result;

    result = thrd_create(&thr, threadfuncA, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    if (result != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    result = thrd_join(thr, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    if (count != 1) {
        t_error("%s failed, count is %d", __func__, count);
    }
    count = 0;
}

/**
 * @tc.name      : thrd_current_0200
 * @tc.desc      : Create two threads to execute the same func
 * @tc.level     : Level 1
 */
void thrd_current_0200(void)
{
    thrd_t thr1, thr2;
    int result;

    result = thrd_create(&thr1, threadfuncB, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_create(&thr2, threadfuncB, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    if (result != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    if (thrd_equal(thr1, thr2)) {
        t_error("%s equal", __func__);
    }

    if (thrd_equal(thr1, thrd_current())) {
        t_error("%s equal", __func__);
    }

    if (thrd_equal(thr2, thrd_current())) {
        t_error("%s equal", __func__);
    }

    result = thrd_join(thr1, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    result = thrd_join(thr2, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    if (count != 2) {
        t_error("%s failed, count is %d", __func__, count);
    }
    count = 0;
}

int main(int argc, char *argv[])
{
    thrd_current_0100();
    thrd_current_0200();
    return t_status;
}
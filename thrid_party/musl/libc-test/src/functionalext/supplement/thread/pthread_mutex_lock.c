/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

static pthread_mutex_t g_mutex;
static int32_t g_count = 0;
static int32_t g_run_max_count = 10000;

void *threadfuncA(void *arg)
{
    int32_t ret = pthread_mutex_lock(&g_mutex);
    EXPECT_EQ("pthread_mutex_lock_0100", ret, 0);
    pthread_mutex_unlock(&g_mutex);
    return arg;
}

void *threadfuncB(void *arg)
{
    int32_t ret = pthread_mutex_lock(&g_mutex);
    ret = pthread_mutex_lock(&g_mutex);
    EXPECT_EQ("pthread_mutex_lock_0200", ret, 0);
    pthread_mutex_unlock(&g_mutex);
    return arg;
}

void *threadfuncC(void *arg)
{
    g_count++;
    int32_t ret = pthread_mutex_lock(&g_mutex);
    if (g_count == INT_MAX) {
        EXPECT_EQ("pthread_mutex_lock_0300", ret, EAGAIN);
    }
    pthread_mutex_unlock(&g_mutex);
    return arg;
}

void *threadfuncD(void *arg)
{
    int32_t ret = pthread_mutex_lock(&g_mutex);
    ret = pthread_mutex_lock(&g_mutex);
    EXPECT_EQ("pthread_mutex_lock_0400", ret, EDEADLK);
    pthread_mutex_unlock(&g_mutex);
    return arg;
}

/**
 * @tc.name:      pthread_mutex_lock_0100
 * @tc.desc:      Verify process pthread_mutex_lock once success when pthread_mutex_init second args is null.
 * @tc.level:     level 0
 */
void pthread_mutex_lock_0100(void)
{
    pthread_mutex_init(&g_mutex, NULL);
    pthread_t tid1;
    int32_t ret = pthread_create(&tid1, NULL, threadfuncA, NULL);
    pthread_join(tid1, NULL);
}

/**
 * @tc.name:      pthread_mutex_lock_0200
 * @tc.desc:      Verify process pthread_mutex_lock twice success when pthread_mutex_init second args is
 *                PTHREAD_MUTEX_NORMAL.
 * @tc.level:     level 1
 */
void pthread_mutex_lock_0200(void)
{
    int32_t type = PTHREAD_MUTEX_RECURSIVE;
    pthread_mutex_init(&g_mutex, (pthread_mutexattr_t *)(&type));
    pthread_t tid2;
    pthread_create(&tid2, NULL, threadfuncB, NULL);
    pthread_join(tid2, NULL);
}

/**
 * @tc.name:      pthread_mutex_lock_0300
 * @tc.desc:      Verify process pthread_mutex_lock once success when pthread_mutex_init second args is
 *                PTHREAD_MUTEX_RECURSIVE.
 * @tc.level:     level 2
 */
void pthread_mutex_lock_0300(void)
{
    int32_t type = PTHREAD_MUTEX_RECURSIVE;
    pthread_mutex_init(&g_mutex, (pthread_mutexattr_t *)(&type));
    pthread_t tid3;
    for (int32_t i = 0; i <= g_run_max_count; i++) {
        pthread_create(&tid3, NULL, threadfuncC, NULL);
        pthread_join(tid3, NULL);
    }
}

/**
 * @tc.name:      pthread_mutex_lock_0400
 * @tc.desc:      Verify process pthread_mutex_lock twice success when pthread_mutex_init second args is
 *                PTHREAD_MUTEX_RECURSIVE.
 * @tc.level:     level 2
 */
void pthread_mutex_lock_0400(void)
{
    int32_t type = PTHREAD_MUTEX_ERRORCHECK;
    pthread_mutex_init(&g_mutex, (pthread_mutexattr_t *)(&type));
    pthread_t tid4;
    pthread_create(&tid4, NULL, threadfuncD, NULL);
    pthread_join(tid4, NULL);
}

int main(void)
{
    pthread_mutex_lock_0100();
    pthread_mutex_lock_0200();
    pthread_mutex_lock_0300();
    pthread_mutex_lock_0400();
    return t_status;
}
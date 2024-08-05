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

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "test.h"

#define TEST(c, ...) ((c) || (t_error(#c " failed: " __VA_ARGS__), 0))
#define SLEEP_20_MS (20000)

pthread_rwlock_t g_rwlock1;
static int number = 0;

static void *pthreadRdlockR1(void *arg)
{
    TEST(pthread_rwlock_rdlock(&g_rwlock1) == 0);
    number++;
    TEST(pthread_rwlock_unlock(&g_rwlock1) == 0);
    return arg;
}

static void *pthreadRdlockR2(void *arg)
{
    usleep(SLEEP_20_MS);
    TEST(pthread_rwlock_rdlock(&g_rwlock1) == 0);
    number--;
    TEST(pthread_rwlock_unlock(&g_rwlock1) == 0);
    return arg;
}

/**
 * @tc.name      : pthread_rwlock_rdlock_0100
 * @tc.desc      : Verify read lock read-write lock success
 * @tc.level     : Level 0
 */
void pthread_rwlock_rdlock_0100(void)
{
    pthread_t thread;
    TEST(pthread_rwlock_init(&g_rwlock1, NULL) == 0);
    TEST(pthread_rwlock_rdlock(&g_rwlock1) == 0);
    TEST(pthread_rwlock_unlock(&g_rwlock1) == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock1) == 0);
}

/**
 * @tc.name      : pthread_rwlock_rdlock_0200
 * @tc.desc      : Verify that the read lock is successful for multiple read-write locks
 * @tc.level     : Level 1
 */
void pthread_rwlock_rdlock_0200(void)
{
    pthread_t thread;
    TEST(pthread_rwlock_init(&g_rwlock1, NULL) == 0);
    TEST(pthread_rwlock_rdlock(&g_rwlock1) == 0);
    TEST(pthread_rwlock_rdlock(&g_rwlock1) == 0);
    TEST(pthread_rwlock_unlock(&g_rwlock1) == 0);
    TEST(pthread_rwlock_unlock(&g_rwlock1) == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock1) == 0);
}

/**
 * @tc.name      : pthread_rwlock_rdlock_0300
 * @tc.desc      : Verify that the read lock read-write lock is successfully set multiple times
 * @tc.level     : Level 1
 */
void pthread_rwlock_rdlock_0300(void)
{
    pthread_t tid[2];
    TEST(pthread_rwlock_init(&g_rwlock1, NULL) == 0);

    TEST(pthread_create(&tid[0], NULL, pthreadRdlockR1, NULL) == 0);
    TEST(pthread_create(&tid[1], NULL, pthreadRdlockR2, NULL) == 0);
    TEST(pthread_join(tid[0], NULL) == 0);
    TEST(pthread_join(tid[1], NULL) == 0);
    TEST(number == 0);
    TEST(pthread_rwlock_destroy(&g_rwlock1) == 0);
}

int main(int argc, char *argv[])
{
    pthread_rwlock_rdlock_0100();
    pthread_rwlock_rdlock_0200();
    pthread_rwlock_rdlock_0300();
    return t_status;
}
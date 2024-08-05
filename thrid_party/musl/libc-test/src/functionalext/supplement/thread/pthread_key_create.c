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

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

static int32_t g_sleep_time = 2;
static pthread_key_t g_key;

void *threadfuncA(void *arg)
{
    sleep(g_sleep_time);
    int32_t value = 0;
    int32_t ret = pthread_setspecific(g_key, &value);
    EXPECT_EQ("pthread_key_create_0100_threadfuncA", ret, 0);
    int32_t *keyRet = (int32_t *)pthread_getspecific(g_key);
    EXPECT_EQ("pthread_key_create_0100_threadfuncA", *keyRet, 0);
    return arg;
}

void *threadfuncB(void *arg)
{
    int32_t value = 0;
    int32_t ret = pthread_setspecific(g_key, &value);
    EXPECT_EQ("pthread_key_create_0200_threadfuncB", ret, 0);
    int32_t *keyRet = (int32_t *)pthread_getspecific(g_key);
    EXPECT_EQ("pthread_key_create_0200_threadfuncB", *keyRet, 0);
    return arg;
}

void testfunc(void *arg)
{}

/**
 * @tc.name:      pthread_key_create_0100
 * @tc.desc:      Verify pthread_key_create process success when second args is null.
 * @tc.desc:      level 0
 */
void pthread_key_create_0100(void)
{
    pthread_t tid1;
    pthread_create(&tid1, NULL, threadfuncA, NULL);
    int32_t ret = pthread_key_create(&g_key, NULL);
    EXPECT_EQ("pthread_key_create_0100", ret, 0);
    pthread_join(tid1, NULL);
    pthread_key_delete(g_key);
}

/**
 * @tc.name:      pthread_key_create_0200
 * @tc.desc:      Verify pthread_key_create process success when second args is not null.
 * @tc.desc:      level 1
 */
void pthread_key_create_0200(void)
{
    pthread_t tid2;
    int32_t ret = pthread_key_create(&g_key, testfunc);
    pthread_create(&tid2, NULL, threadfuncB, NULL);
    pthread_join(tid2, NULL);
    EXPECT_EQ("pthread_key_create_0200", ret, 0);
}

int main(void)
{
    pthread_key_create_0100();
    pthread_key_create_0200();
    return t_status;
}
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
#include <semaphore.h>
#include "functionalext.h"

static int32_t priorityTen = 10;
static int32_t priorityHundred = 100;
static sem_t g_sem;

void *threadfuncA(void *arg)
{
    sem_wait(&g_sem);
    return arg;
}

/**
 * @tc.name      : pthread_setschedparam_0100
 * @tc.desc      : Verify pthread_setschedparam process success when scheduling policy is SCHED_OTHER and priority is 0
 * @tc.level     : Level 1
 */
void pthread_setschedparam_0100(void)
{
    pthread_t tid;
    sem_init(&g_sem, 0, 0);
    pthread_create(&tid, NULL, threadfuncA, NULL);
    struct sched_param sched;
    sched.sched_priority = 0;
    int32_t ret = pthread_setschedparam(tid, SCHED_OTHER, &sched);
    EXPECT_EQ("pthread_setschedparam_0100", ret, 0);
    int32_t semRet = sem_post(&g_sem);
    EXPECT_EQ("pthread_setschedparam_0100", semRet, 0);
    pthread_join(tid, NULL);
    sem_destroy(&g_sem);
}

int main(void)
{
    pthread_setschedparam_0100();
    return t_status;
}
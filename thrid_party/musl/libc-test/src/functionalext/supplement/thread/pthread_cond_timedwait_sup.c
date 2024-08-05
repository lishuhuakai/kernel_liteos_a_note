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
#include <sys/time.h>
#include <unistd.h>
#include "functionalext.h"
int flag = 0;
static pthread_cond_t gcond;
static pthread_mutex_t gmutex;
struct timespec gtm;

void *threadfuncA(void *arg)
{
    struct timeval now;
    struct timespec outtime;
    gtm.tv_nsec = 1;
    pthread_mutex_lock(&gmutex);
    gettimeofday(&now, NULL);
    outtime.tv_sec = now.tv_sec + 5;
    outtime.tv_nsec = now.tv_usec * 1000;
    pthread_cond_timedwait(&gcond, &gmutex, &outtime);
    flag++;
    pthread_mutex_unlock(&gmutex);
    return arg;
}

void *threadfuncB(void *arg)
{
    pthread_mutex_lock(&gmutex);
    pthread_cond_signal(&gcond);
    pthread_mutex_unlock(&gmutex);
    return arg;
}

/**
 * @tc.name:      pthread_cond_timedwait_0100
 * @tc.desc:      Verify pthread create process success
 * @tc.level:     level 0.
 */
void pthread_cond_timedwait_0100(void)
{
    pthread_t pid1, pid2;
    pthread_create(&pid1, NULL, threadfuncA, NULL);
    pthread_create(&pid2, NULL, threadfuncB, NULL);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    EXPECT_EQ("pthread_cond_timedwait_0200", flag, 1);
    pthread_mutex_destroy(&gmutex);
    pthread_cond_destroy(&gcond);
}

int main(void)
{
    pthread_cond_timedwait_0100();
    return t_status;
}

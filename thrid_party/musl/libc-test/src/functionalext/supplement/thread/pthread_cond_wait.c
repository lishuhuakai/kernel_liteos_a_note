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
#include "test.h"

pthread_cond_t cond;
pthread_mutex_t mutex;
int flag = 0;
pthread_t pid1;
pthread_t pid2;

void *threadfunc1(void *arg)
{
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    flag++;
    return 0;
}

void *threadfunc2(void *arg)
{
    sleep(1);
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    if (flag != 1) {
        t_error("pthread_cond_wait_0100 pthread_cond_wait error get flag is %d are not want 1\n", flag);
    }
    return 0;
}

/**
 * @tc.name      : pthread_cond_wait_0100
 * @tc.desc      : The parameter is valid, verify that the condition variable is activated
 * @tc.level     : Level 0
 */
void pthread_cond_wait_0100(void)
{
    pthread_create(&pid1, NULL, threadfunc1, NULL);
    pthread_create(&pid2, NULL, threadfunc2, NULL);
}

int main(int argc, char *argv[])
{
    pthread_cond_wait_0100();
    return t_status;
}
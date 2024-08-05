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
#include <semaphore.h>
#include <limits.h>

#include "functionalext.h"

#define THREADNUM 4

pthread_barrier_t mybarrier;

static void threadfunc(void)
{
    sleep(1);
    int ret = pthread_barrier_wait(&mybarrier);
    EXPECT_NE("pthread_barrier_wait_0100", ret, ERREXPECT);

    return;
}

/**
 * @tc.name      : pthread_barrier_wait_0100
 * @tc.desc      : Blocking and waiting for 4 sub-threads, the return value of the calling function is successful
 * @tc.level     : Level 1
 */
void pthread_barrier_wait_0100(void)
{
    int i = 0;
    pthread_t ids[THREADNUM];

    int ret = pthread_barrier_init(&mybarrier, NULL, THREADNUM + 1);
    EXPECT_EQ("pthread_barrier_wait_0100", ret, CMPFLAG);

    for (i = 0; i < THREADNUM; i++) {
        if (pthread_create(&ids[i], NULL, (void *)threadfunc, NULL) != 0) {
            t_error("create thread failed\n");
            return;
        }
    }

    ret = pthread_barrier_wait(&mybarrier);

    for (i = 0; i < THREADNUM; i++) {
        ret = pthread_join(ids[i], NULL);
        if (ret != 0) {
            t_error("thread wait failed\n");
            return;
        }
    }

    ret = pthread_barrier_destroy(&mybarrier);
    EXPECT_EQ("pthread_barrier_wait_0100", ret, CMPFLAG);
}

/**
 * @tc.name      : pthread_barrier_wait_0200
 * @tc.desc      : The number of blocked threads is 1, and the calling function returns ERREXPECT
 * @tc.level     : Level 2
 */
void pthread_barrier_wait_0200(void)
{
    int threadnum = 1;

    int ret = pthread_barrier_init(&mybarrier, NULL, threadnum);
    EXPECT_EQ("pthread_barrier_wait_0200", ret, CMPFLAG);

    ret = pthread_barrier_wait(&mybarrier);
    EXPECT_EQ("pthread_barrier_wait_0200", ret, ERREXPECT);

    ret = pthread_barrier_destroy(&mybarrier);
    EXPECT_EQ("pthread_barrier_wait_0200", ret, CMPFLAG);
}

int main(void)
{
    pthread_barrier_wait_0100();
    pthread_barrier_wait_0200();

    return t_status;
}
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

/**
 * @tc.name      : pthread_barrier_init_0100
 * @tc.desc      : Pass a reasonable number of threads to determine whether the return value of the function is normal
 * @tc.level     : Level 0
 */
void pthread_barrier_init_0100(void)
{
    pthread_barrier_t mybarrier;
    unsigned int threadnum = 4;

    int ret = pthread_barrier_init(&mybarrier, NULL, threadnum);
    EXPECT_EQ("pthread_barrier_init_0100", ret, CMPFLAG);

    ret = pthread_barrier_destroy(&mybarrier);
    EXPECT_EQ("pthread_barrier_init_0100", ret, CMPFLAG);
}

/**
 * @tc.name      : pthread_barrier_init_0200
 * @tc.desc      : Pass the abnormal minimum values to determine whether the return value of
 *                 the function is EINVAL
 * @tc.level     : Level 2
 */
void pthread_barrier_init_0200(void)
{
    pthread_barrier_t mybarrier;
    int threadnum = 0;

    int ret = pthread_barrier_init(&mybarrier, NULL, threadnum);
    EXPECT_EQ("pthread_barrier_init_0200", ret, EINVAL);

    ret = pthread_barrier_destroy(&mybarrier);
    EXPECT_EQ("pthread_barrier_init_0200", ret, CMPFLAG);
}

int main(void)
{
    pthread_barrier_init_0100();
    pthread_barrier_init_0200();
    return t_status;
}
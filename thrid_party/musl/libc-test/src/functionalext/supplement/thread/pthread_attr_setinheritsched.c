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

#include "functionalext.h"

/**
 * @tc.name      : pthread_attr_setinheritsched_0100
 * @tc.desc      : Verify that the preset properties can be obtained through get inheritsched in a single-threaded
 * environment
 * @tc.level     : Level 0
 */
void pthread_attr_setinheritsched_0100(void)
{
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
    int result = pthread_attr_getinheritsched(&attr, &result);
    EXPECT_EQ("pthread_attr_setinheritsched_0100", result, PTHREAD_INHERIT_SCHED);

    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_getinheritsched(&attr, &result);
    EXPECT_EQ("pthread_attr_setinheritsched_0100", result, PTHREAD_EXPLICIT_SCHED);

    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_setinheritsched_0200
 * @tc.desc      : Verify that the return value is EINVAL when setting an exception value in a single-threaded
 * environment
 * @tc.level     : Level 2
 */
void pthread_attr_setinheritsched_0200(void)
{
    pthread_attr_t attr;
    int inheritsched = 2;
    int result = -1;

    int ret = pthread_attr_init(&attr);
    EXPECT_EQ("pthread_attr_setinheritsched_0200", ret, CMPFLAG);

    ret = pthread_attr_setinheritsched(&attr, inheritsched);
    EXPECT_EQ("pthread_attr_setinheritsched_0200", ret, EINVAL);

    pthread_attr_destroy(&attr);
}

int main(void)
{
    pthread_attr_setinheritsched_0100();
    pthread_attr_setinheritsched_0200();

    return t_status;
}
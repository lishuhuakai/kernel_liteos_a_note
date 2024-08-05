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
#include "functionalext.h"

/**
 * @tc.name      : pthread_mutexattr_setprotocol_0100
 * @tc.desc      : Correct parameter setting condition variable property value
 * @tc.level     : Level 0
 */
void pthread_mutexattr_setprotocol_0100(void)
{
    pthread_mutexattr_t attr;
    int ret = pthread_mutexattr_init(&attr);
    EXPECT_EQ("pthread_mutexattr_setprotocol_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_NONE);
    EXPECT_EQ("pthread_mutexattr_setprotocol_0100", ret, CMPFLAG);

    ret = pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_PROTECT);
    EXPECT_EQ("pthread_mutexattr_setprotocol_0100", ret, ENOTSUP);

    pthread_mutexattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_mutexattr_setprotocol_0200
 * @tc.desc      : Exception parameter sets mutex variable property value
 * @tc.level     : Level 2
 */
void pthread_mutexattr_setprotocol_0200(void)
{
    pthread_mutexattr_t attr;
    int error = PTHREAD_PRIO_PROTECT + 1;

    int ret = pthread_mutexattr_init(&attr);
    EXPECT_EQ("pthread_mutexattr_setprotocol_0200", ret, CMPFLAG);

    ret = pthread_mutexattr_setprotocol(&attr, error);
    EXPECT_EQ("pthread_mutexattr_setprotocol_0200", ret, EINVAL);

    pthread_mutexattr_destroy(&attr);
}

int main(void)
{
    pthread_mutexattr_setprotocol_0100();
    pthread_mutexattr_setprotocol_0200();
    return t_status;
}
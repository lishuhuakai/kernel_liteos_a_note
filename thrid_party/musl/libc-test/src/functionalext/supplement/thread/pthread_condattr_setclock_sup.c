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

#define TEST_CONDATTR_VALUE 60
#define TEST_CONDATTR_ERROR_VALUE 3

/**
 * @tc.name      : pthread_condattr_setclock_0100
 * @tc.desc      : Correct parameters, set the clock property of the condition variable
 * @tc.level     : Level 0
 */
void pthread_condattr_setclock_0100(void)
{
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    int ret = pthread_condattr_setclock(&attr, TEST_CONDATTR_VALUE);
    EXPECT_EQ("pthread_condattr_setclock_0100", ret, CMPFLAG);
    pthread_condattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_condattr_setclock_0200
 * @tc.desc      : Error parameter, set the clock property of the condition variable
 * @tc.level     : Level 0
 */
void pthread_condattr_setclock_0200(void)
{
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    int ret = pthread_condattr_setclock(&attr, -1);
    EXPECT_EQ("pthread_condattr_setclock_0200", ret, EINVAL);

    ret = pthread_condattr_setclock(&attr, TEST_CONDATTR_ERROR_VALUE);
    EXPECT_EQ("pthread_condattr_setclock_0200", ret, EINVAL);

    pthread_condattr_destroy(&attr);
}

int main(void)
{
    pthread_condattr_setclock_0100();
    pthread_condattr_setclock_0200();
    return t_status;
}
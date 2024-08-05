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
 * @tc.name      : pthread_rwlockattr_setpshared_0100
 * @tc.desc      : Get thread scope
 * @tc.level     : Level 0
 */
void pthread_rwlockattr_setpshared_0100(void)
{
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    int rev = pthread_rwlockattr_setpshared(&attr, 1);
    EXPECT_EQ("pthread_rwlockattr_setpshared_0100", rev, 0);
    pthread_rwlockattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_rwlockattr_setpshared_0200
 * @tc.desc      : Failed to get thread scope
 * @tc.level     : Level 2
 */
void pthread_rwlockattr_setpshared_0200(void)
{
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    int rev = pthread_rwlockattr_setpshared(&attr, 2);
    EXPECT_EQ("pthread_rwlockattr_setpshared_0200", rev, EINVAL);
    pthread_rwlockattr_destroy(&attr);
}

int main(void)
{
    pthread_rwlockattr_setpshared_0100();
    pthread_rwlockattr_setpshared_0200();
    return t_status;
}
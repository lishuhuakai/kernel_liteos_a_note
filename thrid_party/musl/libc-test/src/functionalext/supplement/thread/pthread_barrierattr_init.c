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
 * @tc.name      : pthread_barrierattr_init_0100
 * @tc.desc      : Initialize barrier pthread properties
 * @tc.level     : Level 0
 */
void pthread_barrierattr_init_0100(void)
{
    pthread_barrierattr_t attr;

    int ret = pthread_barrierattr_init(&attr);
    EXPECT_EQ("pthread_barrierattr_init_0100", ret, 0);

    ret = pthread_barrierattr_destroy(&attr);
}

int main(void)
{
    pthread_barrierattr_init_0100();

    return t_status;
}
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

#define TEST_COND_ATTR_VALUE_MIN (-1)
#define TEST_COND_ATTR_VALUE_MAX 2

/**
 * @tc.name      : pthread_barrierattr_setpshared_0100
 * @tc.desc      : Correct parameter setting barrier variable property value
 * @tc.level     : Level 0
 */
void pthread_barrierattr_setpshared_0100(void)
{
    pthread_barrierattr_t attr;
    int result = -1;
    int shared = 0;

    int ret = pthread_barrierattr_init(&attr);
    EXPECT_EQ("pthread_barrierattr_setpshared_0100", ret, CMPFLAG);
    ret = pthread_barrierattr_setpshared(&attr, shared);
    EXPECT_EQ("pthread_barrierattr_setpshared_0100", ret, CMPFLAG);

    shared = 1;
    ret = pthread_barrierattr_setpshared(&attr, shared);
    EXPECT_EQ("pthread_barrierattr_setpshared_0100", ret, CMPFLAG);

    pthread_barrierattr_destroy(&attr);
}

/**
 * @tc.name      : pthread_barrierattr_setpshared_0200
 * @tc.desc      : Exception parameter sets barrier variable property value
 * @tc.level     : Level 2
 */
void pthread_barrierattr_setpshared_0200(void)
{
    pthread_barrierattr_t attr;

    int ret = pthread_barrierattr_init(&attr);
    EXPECT_EQ("pthread_barrierattr_setpshared_0200", ret, 0);

    ret = pthread_barrierattr_setpshared(&attr, TEST_COND_ATTR_VALUE_MIN);
    EXPECT_EQ("pthread_barrierattr_setpshared_0200", ret, EINVAL);

    ret = pthread_barrierattr_setpshared(&attr, TEST_COND_ATTR_VALUE_MAX);
    EXPECT_EQ("pthread_barrierattr_setpshared_0200", ret, EINVAL);

    pthread_barrierattr_destroy(&attr);
}

int main(void)
{
    pthread_barrierattr_setpshared_0100();
    pthread_barrierattr_setpshared_0200();

    return t_status;
}
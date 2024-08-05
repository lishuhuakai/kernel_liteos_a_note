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
#include <stdlib.h>
#include "functionalext.h"

#define TEST_GUARD_SIZE 128

/**
 * @tc.name      : pthread_attr_setguardsize_0100
 * @tc.desc      : Set thread property threshold value
 * @tc.level     : Level 0
 */
void pthread_attr_setguardsize_0100(void)
{
    int scope = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int rev = pthread_attr_setguardsize(&attr, TEST_GUARD_SIZE);
    EXPECT_EQ("pthread_attr_setguardsize_0100", rev, 0);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_attr_setguardsize_0200
 * @tc.desc      : Use an excessively large value, set the thread property threshold value
 * @tc.level     : Level 2
 */
void pthread_attr_setguardsize_0200(void)
{
    int scope = 0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int rev = pthread_attr_setguardsize(&attr, SIZE_MAX);
    EXPECT_EQ("pthread_attr_setguardsize_0200", rev, EINVAL);
    pthread_attr_destroy(&attr);
}

int main(void)
{
    pthread_attr_setguardsize_0100();
    pthread_attr_setguardsize_0200();
    return t_status;
}
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
 * @tc.name      : pthread_attr_setschedpolicy_0100
 * @tc.desc      : Set thread scheduling policy
 * @tc.level     : Level 0
 */
void pthread_attr_setschedpolicy_0100(void)
{
    pthread_attr_t attr;
    int setpolicy = 1;
    int getpolicy = 0;
    pthread_attr_init(&attr);
    int ret = pthread_attr_setschedpolicy(&attr, setpolicy);
    EXPECT_EQ("pthread_attr_setschedpolicy_0100", ret, CMPFLAG);

    ret = pthread_attr_getschedpolicy(&attr, &getpolicy);
    EXPECT_EQ("pthread_attr_setschedpolicy_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_attr_setschedpolicy_0100", getpolicy, setpolicy);
    pthread_attr_destroy(&attr);
}

int main(void)
{
    pthread_attr_setschedpolicy_0100();
    return t_status;
}
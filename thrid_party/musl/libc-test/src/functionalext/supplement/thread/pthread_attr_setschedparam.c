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
 * @tc.name      : pthread_attr_setschedparam_0100
 * @tc.desc      : Set scheduling parameter attributes in thread attributes object
 * @tc.level     : Level 0
 */
void pthread_attr_setschedparam_0100(void)
{
    pthread_attr_t attr;
    struct sched_param setparam;
    struct sched_param getparam;
    setparam.sched_priority = 1;

    pthread_attr_init(&attr);
    int ret = pthread_attr_setschedparam(&attr, &setparam);
    EXPECT_EQ("pthread_attr_setschedparam_0100", ret, CMPFLAG);

    getparam.sched_priority = 0;
    ret = pthread_attr_getschedparam(&attr, &getparam);
    EXPECT_EQ("pthread_attr_setschedparam_0100", ret, CMPFLAG);
    EXPECT_EQ("pthread_attr_setschedparam_0100", getparam.sched_priority, setparam.sched_priority);
    pthread_attr_destroy(&attr);
}

int main(void)
{
    pthread_attr_setschedparam_0100();
    return t_status;
}
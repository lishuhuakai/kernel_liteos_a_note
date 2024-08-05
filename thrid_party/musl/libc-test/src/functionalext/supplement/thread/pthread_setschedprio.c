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

static void Thread1(void)
{
    sleep(1);
    int ret = pthread_setschedprio(pthread_self(), 10);
    EXPECT_EQ("pthread_setschedprio_0100", ret, 0);
}

static void Thread2(void)
{
    sleep(1);

    int ret = pthread_setschedprio(pthread_self(), 1000);
    EXPECT_EQ("pthread_setschedprio_0100", ret, EINVAL);
}

static void setthreadproperty(pthread_attr_t *attr, struct sched_param *param)
{
    int ret = pthread_attr_init(attr);
    EXPECT_EQ("pthread_setschedprio_0100", ret, CMPFLAG);

    param->sched_priority = 51;
    ret = pthread_attr_setschedpolicy(attr, SCHED_RR);
    EXPECT_EQ("pthread_setschedprio_0100", ret, CMPFLAG);
    ret = pthread_attr_setschedparam(attr, param);
    EXPECT_EQ("pthread_setschedprio_0100", ret, CMPFLAG);

    ret = pthread_attr_setinheritsched(attr, PTHREAD_EXPLICIT_SCHED);
    EXPECT_EQ("pthread_setschedprio_0100", ret, CMPFLAG);
}

/**
 * @tc.name      : pthread_setschedprio_0100
 * @tc.desc      : Set the thread properties, and judge the normal system of the function in the child thread
 * @tc.level     : Level 0
 */
void pthread_setschedprio_0100(void)
{
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param param;

    setthreadproperty(&attr, &param);
    int ret = pthread_create(&thread, &attr, (void *)Thread1, NULL);
    if (ret != 0) {
        EXPECT_EQ("pthread_setschedprio_0100", ret, CMPFLAG);
        return;
    }

    pthread_join(thread, NULL);
    pthread_attr_destroy(&attr);
}

/**
 * @tc.name      : pthread_setschedprio_0200
 * @tc.desc      : Set the thread properties, and judge abnormal system of the function in the child thread
 * @tc.level     : Level 2
 */
void pthread_setschedprio_0200(void)
{
    pthread_t thread;
    pthread_attr_t attr;
    struct sched_param param;

    setthreadproperty(&attr, &param);
    int ret = pthread_create(&thread, &attr, (void *)Thread2, NULL);
    if (ret != 0) {
        EXPECT_EQ("pthread_setschedprio_0200", ret, CMPFLAG);
        return;
    }

    pthread_join(thread, NULL);
    pthread_attr_destroy(&attr);
}

int main(void)
{
    pthread_setschedprio_0100();
    pthread_setschedprio_0200();

    return 0;
}

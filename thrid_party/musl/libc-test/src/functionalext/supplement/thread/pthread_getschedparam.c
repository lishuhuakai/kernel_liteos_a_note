/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

void *threadfuncA(void *arg)
{
    return arg;
}

/**
 * @tc.name      : pthread_getschedparam_0100
 * @tc.desc      : Verify pthread_getschedparam process success
 * @tc.level     : Level 1
 */
void pthread_getschedparam_0100(void)
{
    pthread_t ph;
    pthread_create(&ph, NULL, threadfuncA, NULL);
    int policy = 0;
    struct sched_param sp;
    int32_t ret = pthread_getschedparam(ph, &policy, &sp);
    void *thread_result = NULL;
    pthread_join(ph, &thread_result);
    EXPECT_EQ("pthread_getschedparam_0100", ret, 0);
}

int main(void)
{
    pthread_getschedparam_0100();
    return t_status;
}

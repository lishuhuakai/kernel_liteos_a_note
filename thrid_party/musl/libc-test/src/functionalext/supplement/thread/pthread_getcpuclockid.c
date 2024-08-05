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
#include <time.h>
#include "functionalext.h"

int gthread_exit_flag = 0;
static void *thread_func(void *arg)
{
    for (;;) {
        if (gthread_exit_flag) {
            break;
        } else {
            continue;
        }
    }
    return (void *)0;
}

/**
 * @tc.name      : pthread_getcpuclockid_0100
 * @tc.desc      : Get the specified thread clock ID
 * @tc.level     : Level 0
 */
void pthread_getcpuclockid_0100(void)
{
    pthread_t thread;
    clockid_t cid;
    int ret = pthread_create(&thread, NULL, thread_func, NULL);
    EXPECT_EQ("pthread_getcpuclockid_0100", ret, 0);
    ret = pthread_getcpuclockid(pthread_self(), &cid);
    EXPECT_EQ("pthread_getcpuclockid_0100", ret, 0);
    EXPECT_TRUE("pthread_getcpuclockid_0100", cid != 0);

    ret = pthread_getcpuclockid(thread, &cid);
    EXPECT_EQ("pthread_getcpuclockid_0100", ret, 0);
    EXPECT_TRUE("pthread_getcpuclockid_0100", cid != 0);
    gthread_exit_flag = 1;
    sleep(1);
}

int main(void)
{
    pthread_getcpuclockid_0100();
    return t_status;
}
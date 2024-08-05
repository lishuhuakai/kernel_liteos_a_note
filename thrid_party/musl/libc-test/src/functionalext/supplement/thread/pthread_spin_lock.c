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
#include <unistd.h>
#include "functionalext.h"

pthread_spinlock_t lock = 0;
volatile int count = 0;

static void thread1(void)
{
    int ret = pthread_spin_lock(&lock);
    EXPECT_EQ("pthread_spin_lock_0100", ret, CMPFLAG);
    ++count;
    ret = pthread_spin_unlock(&lock);
    pthread_exit("success");
}

/**
 * @tc.name      : pthread_spin_lock_0100
 * @tc.desc      : Verify that the return value of the calling function is correct
 * @tc.level     : Level 0
 */
void pthread_spin_lock_0100(void)
{
    pthread_spinlock_t spin;

    int ret = pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);
    EXPECT_EQ("pthread_spin_lock_0100", ret, CMPFLAG);

    ret = pthread_spin_lock(&spin);
    EXPECT_EQ("pthread_spin_lock_0100", ret, CMPFLAG);

    ret = pthread_spin_unlock(&spin);
    EXPECT_EQ("pthread_spin_lock_0100", ret, CMPFLAG);

    pthread_spin_destroy(&spin);
}

/**
 * @tc.name      : pthread_spin_lock_0200
 * @tc.desc      : Verify that under multi-threading, whether the modification of the global variable data is
 *                 as expected, and whether the spin lock is successfully locked
 * @tc.level     : Level 0
 */
void pthread_spin_lock_0200(void)
{
    pthread_t thread;
    void *rev;

    int ret = pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);

    EXPECT_EQ("pthread_spin_lock_0200", ret, CMPFLAG);
    ret = pthread_create(&thread, NULL, (void *)thread1, NULL);
    if (ret != 0) {
        EXPECT_EQ("pthread_spin_lock_0200", ret, CMPFLAG);
        return;
    }

    ret = pthread_spin_lock(&lock);
    EXPECT_EQ("pthread_spin_lock_0200", ret, CMPFLAG);

    ++count;
    ret = pthread_spin_unlock(&lock);

    pthread_join(thread, &rev);
    EXPECT_STREQ("pthread_spin_lock_0200", rev, "success");
    EXPECT_EQ("pthread_spin_lock_0200", count, 2);

    pthread_spin_destroy(&lock);
}

int main(void)
{
    pthread_spin_lock_0100();
    pthread_spin_lock_0200();

    return t_status;
}
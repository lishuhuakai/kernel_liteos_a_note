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
#include <threads.h>
#include <time.h>
#include "functionalext.h"

const int TWO = 2;

extern int __mtx_timedlock_time64(mtx_t *__restrict, const struct timespec *__restrict);

/**
 * @tc.name      : mtx_timedlock_0100
 * @tc.desc      : Provide correct parameters, test timeout and lock
 * @tc.level     : Level 0
 */
void mtx_timedlock_0100(void)
{
    struct timespec timeout;
    timeout.tv_sec = TWO;
    timeout.tv_nsec = 0;
    mtx_t mutex;
    int ret = 0;
    ret = mtx_init(&mutex, mtx_timed);
    if (ret != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    ret = mtx_timedlock(&mutex, &timeout);
    EXPECT_EQ("mtx_timedlock_0100", ret, thrd_success);
    ret = mtx_unlock(&mutex);
    if (ret != thrd_success) {
        t_error("%s mtx_unlock failed", __func__);
        return;
    }
    mtx_destroy(&mutex);
}

/**
 * @tc.name      : mtx_timedlock_0200
 * @tc.desc      : In the locked state, the test times out and locks
 * @tc.level     : Level 2
 */
void mtx_timedlock_0200(void)
{
    mtx_t mutex;
    struct timespec timeout;
    timeout.tv_sec = TWO;
    timeout.tv_nsec = 0;
    int ret = 0;
    ret = mtx_init(&mutex, mtx_timed);
    if (ret != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    if (ret != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    ret = mtx_lock(&mutex);
    ret = mtx_timedlock(&mutex, &timeout);
    EXPECT_EQ("mtx_timedlock_0200", ret, thrd_timedout);
    ret = mtx_unlock(&mutex);
    if (ret != thrd_success) {
        t_error("%s mtx_unlock failed", __func__);
        return;
    }
    mtx_destroy(&mutex);
}

/**
 * @tc.name      : mtx_timedlock_time64_0100
 * @tc.desc      : Provide correct parameters, test timeout and lock
 * @tc.level     : Level 0
 */
void mtx_timedlock_time64_0100(void)
{
    struct timespec timeout;
    timeout.tv_sec = TWO;
    timeout.tv_nsec = 0;
    mtx_t mutex;
    int ret = 0;
    ret = mtx_init(&mutex, mtx_timed);
    if (ret != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    ret = __mtx_timedlock_time64(&mutex, &timeout);
    EXPECT_EQ("mtx_timedlock_time64_0100", ret, thrd_success);
    ret = mtx_unlock(&mutex);
    if (ret != thrd_success) {
        t_error("%s mtx_unlock failed", __func__);
        return;
    }
    mtx_destroy(&mutex);
}

int main(void)
{
    mtx_timedlock_0100();
    mtx_timedlock_0200();
    mtx_timedlock_time64_0100();
    return t_status;
}
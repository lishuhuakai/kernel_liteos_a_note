/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include <threads.h>
#include "functionalext.h"

static cnd_t cond;
static mtx_t mutex;

extern int __cnd_timedwait_time64(cnd_t *__restrict, mtx_t *__restrict, const struct timespec *__restrict);

static int signal_parent(void *arg)
{
    if (mtx_lock(&mutex) != thrd_success) {
        t_error("%s mtx_lock failed\n", __func__);
    }
    if (cnd_signal(&cond) != thrd_success) {
        t_error("%s cnd_signal failed\n", __func__);
    }
    if (mtx_unlock(&mutex) != thrd_success) {
        t_error("%s mtx_unlock failed\n", __func__);
    }

    thrd_exit(thrd_success);
}

/**
 * @tc.name      : cnd_timedwait_0100
 * @tc.desc      : The parameter is valid and can block the calling thread and release the specified mutex.
 * @tc.level     : Level 0
 */
void cnd_timedwait_0100(void)
{
    thrd_t id;
    struct timespec w_time;

    if (cnd_init(&cond) != thrd_success) {
        t_error("%s cnd_init failed\n", __func__);
    }
    if (mtx_init(&mutex, mtx_plain) != thrd_success) {
        t_error("%s mtx_init failed\n", __func__);
    }
    if (mtx_lock(&mutex) != thrd_success) {
        t_error("%s mtx_lock failed\n", __func__);
    }
    if (clock_gettime(CLOCK_REALTIME, &w_time) != 0) {
        t_error("%s clock_gettime failed\n", __func__);
    }

    w_time.tv_sec += 3600;

    if (thrd_create(&id, signal_parent, NULL) != thrd_success) {
        t_error("%s thrd_create failed\n", __func__);
    }

    if (cnd_timedwait(&cond, &mutex, &w_time) != thrd_success) {
        t_error("%s cnd_timedwait failed\n", __func__);
    }

    if (thrd_join(id, NULL) != thrd_success) {
        t_error("%s thrd_join failed\n", __func__);
    }

    if (mtx_unlock(&mutex) != thrd_success) {
        t_error("%s mtx_unlock failed\n", __func__);
    }
    mtx_destroy(&mutex);
    cnd_destroy(&cond);
}

/**
 * @tc.name      : cnd_timedwait_time64_0100
 * @tc.desc      : The parameter is valid and can block the calling thread and release the specified mutex.
 * @tc.level     : Level 0
 */
void cnd_timedwait_time64_0100(void)
{
    thrd_t id;
    struct timespec w_time;

    if (cnd_init(&cond) != thrd_success) {
        t_error("%s cnd_init failed\n", __func__);
    }
    if (mtx_init(&mutex, mtx_plain) != thrd_success) {
        t_error("%s mtx_init failed\n", __func__);
    }
    if (mtx_lock(&mutex) != thrd_success) {
        t_error("%s mtx_lock failed\n", __func__);
    }
    if (clock_gettime(CLOCK_REALTIME, &w_time) != 0) {
        t_error("%s clock_gettime failed\n", __func__);
    }

    w_time.tv_sec += 3600;

    if (thrd_create(&id, signal_parent, NULL) != thrd_success) {
        t_error("%s thrd_create failed\n", __func__);
    }

    if (__cnd_timedwait_time64(&cond, &mutex, &w_time) != thrd_success) {
        t_error("%s cnd_timedwait failed\n", __func__);
    }

    if (thrd_join(id, NULL) != thrd_success) {
        t_error("%s thrd_join failed\n", __func__);
    }

    if (mtx_unlock(&mutex) != thrd_success) {
        t_error("%s mtx_unlock failed\n", __func__);
    }
    mtx_destroy(&mutex);
    cnd_destroy(&cond);
}

int main(int argc, char *argv[])
{
    cnd_timedwait_0100();
    cnd_timedwait_time64_0100();
    return t_status;
}
/*
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

#include <stdbool.h>
#include <threads.h>
#include "test.h"

static cnd_t cond;
static mtx_t mutex;
static unsigned int waiting_threads;

#define N 3

static int child_wait(void *data)
{
    mtx_lock(&mutex);
    ++waiting_threads;
    cnd_wait(&cond, &mutex);
    mtx_unlock(&mutex);

    thrd_exit(thrd_success);
}

/**
 * @tc.name      : cnd_broadcast_0100
 * @tc.desc      :
 * @tc.level     : Level 0
 */
void cnd_broadcast_0100(void)
{
    thrd_t ids[N];
    unsigned char i;

    if (cnd_init(&cond) != thrd_success) {
        t_error("%s cnd_init failed\n", __func__);
    }

    if (mtx_init(&mutex, mtx_plain) != thrd_success) {
        t_error("%s mtx_init failed\n", __func__);
    }

    for (i = 0; i < N; ++i) {
        if (thrd_create(&ids[i], child_wait, NULL) != thrd_success) {
            t_error("%s thrd_create failed\n", __func__);
        }
    }

    while (true) {
        mtx_lock(&mutex);
        if (waiting_threads > N) {
            t_error("%s ", __func__);
        }
        bool done_waiting = waiting_threads == N;
        mtx_unlock(&mutex);
        if (done_waiting) {
            break;
        }
        thrd_sleep(&((struct timespec){.tv_nsec = 100 * 1000 * 1000}), NULL);
    }

    mtx_lock(&mutex);
    if (cnd_broadcast(&cond) != thrd_success) {
        t_error("%s cnd_broadcast failed\n", __func__);
    }
    mtx_unlock(&mutex);

    for (i = 0; i < N; ++i) {
        if (thrd_join(ids[i], NULL) != thrd_success) {
            t_error("%s thrd_join failed\n", __func__);
        }
    }

    mtx_destroy(&mutex);
    cnd_destroy(&cond);
}

int main(int argc, char *argv[])
{
    cnd_broadcast_0100();
    return t_status;
}
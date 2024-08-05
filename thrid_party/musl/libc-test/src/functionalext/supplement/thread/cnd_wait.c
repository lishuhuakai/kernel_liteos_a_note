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

mtx_t mtx;
cnd_t cnd;
void cnd_waitfirst(void *arg)
{
    int res = -1;
    mtx_lock(&mtx);
    res = cnd_wait(&cnd, &mtx);
    EXPECT_EQ("cnd_wait_0100", res, thrd_success);
    mtx_unlock(&mtx);
}

/**
 * @tc.name      : cnd_wait_0100
 * @tc.desc      : The parameter is valid and can block the calling thread and release the specified mutex
 * @tc.level     : Level 0
 */
void cnd_wait_0100(void)
{
    thrd_t id1;
    int ret;
    cnd_init(&cnd);
    mtx_init(&mtx, 0);
    ret = thrd_create(&id1, (thrd_start_t)cnd_waitfirst, (void *)1);
    EXPECT_EQ("cnd_wait_0100", ret, 0);
    sleep(2);
    mtx_lock(&mtx);
    cnd_signal(&cnd);
    mtx_unlock(&mtx);
    thrd_join(id1, NULL);
    cnd_destroy(&cnd);
    mtx_destroy(&mtx);
}

int main(int argc, char *argv[])
{
    cnd_wait_0100();
    return t_status;
}
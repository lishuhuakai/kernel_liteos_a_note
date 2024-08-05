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

#include <stdlib.h>
#include <threads.h>
#include <unistd.h>
#include "functionalext.h"

static mtx_t g_mtex;

int threadfuncA(void *arg)
{
    int32_t ret = 0;
    ret = mtx_lock(&g_mtex);
    ret = mtx_trylock(&g_mtex);
    EXPECT_EQ("mtx_lock_0100", ret, thrd_success);
    ret = mtx_unlock(&g_mtex);
    EXPECT_EQ("mtx_lock_0100", ret, thrd_success);
    return 0;
}

int threadfuncB(void *arg)
{
    int32_t ret = 0;
    ret = mtx_lock(&g_mtex);
    ret = mtx_trylock(&g_mtex);
    EXPECT_EQ("mtx_lock_0200", ret, thrd_busy);
    ret = mtx_unlock(&g_mtex);
    EXPECT_EQ("mtx_lock_0200", ret, thrd_success);
    return 0;
}

/**
 * @tc.name      : mtx_trylock_0100
 * @tc.desc      : Verify process mtx_trylock success when mtx_init second args is mtx_plain | tx_recursive.
 * @tc.level     : Level 0
 */
void mtx_trylock_0100(void)
{
    int result = mtx_init(&g_mtex, mtx_plain | mtx_recursive);
    if (result != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    thrd_t tid1;
    result = thrd_create(&tid1, threadfuncA, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
        return;
    }
    result = thrd_join(tid1, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
        return;
    }
}

/**
 * @tc.name      : mtx_trylock_0200
 * @tc.desc      : Verify process mtx_trylock fail when mtx_init second args is mtx_plain.
 * @tc.level     : Level 2
 */
void mtx_trylock_0200(void)
{
    int result = mtx_init(&g_mtex, mtx_plain);
    if (result != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    thrd_t tid2;
    result = thrd_create(&tid2, threadfuncB, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
        return;
    }
    result = thrd_join(tid2, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
        return;
    }
}

int main(int argc, char *argv[])
{
    mtx_trylock_0100();
    mtx_trylock_0200();
    return t_status;
}
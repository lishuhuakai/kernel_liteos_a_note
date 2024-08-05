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

static mtx_t g_mtx;

int threadfuncA(void *arg)
{
    int ret= mtx_lock(&g_mtx);
    EXPECT_EQ("mtx_unlock_0100", ret, 0);
    ret = mtx_unlock(&g_mtx);
    EXPECT_EQ("mtx_unlock_0100", ret, 0);
    return 0;
}

/**
 * @tc.name      : mtx_unlock_0100.
 * @tc.desc      : Verify process mtx_unlock once success.
 * @tc.level     : Level 0.
 */
void mtx_unlock_0100(void)
{
    int result = mtx_init(&g_mtx, mtx_plain);
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

int main(void)
{
    mtx_unlock_0100();
    return t_status;
}
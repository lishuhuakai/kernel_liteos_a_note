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
    EXPECT_EQ("mtx_lock_0100", ret, thrd_success);
    ret = mtx_unlock(&g_mtex);
    EXPECT_EQ("mtx_lock_0100", ret, thrd_success);
    return 0;
}

int threadfuncB(void *arg)
{
    int32_t ret = 0;
    ret = mtx_lock(&g_mtex);
    EXPECT_EQ("mtx_lock_0200", ret, thrd_success);
    ret = mtx_unlock(&g_mtex);
    EXPECT_EQ("mtx_lock_0200", ret, thrd_success);
    return 0;
}

int threadfuncC(void *arg)
{
    int32_t ret = 0;
    ret = mtx_lock(&g_mtex);
    EXPECT_EQ("mtx_lock_0300", ret, thrd_success);
    ret = mtx_unlock(&g_mtex);
    EXPECT_EQ("mtx_lock_0300", ret, thrd_success);
    return 0;
}

int threadfuncD(void *arg)
{
    int32_t ret = 0;
    ret = mtx_lock(&g_mtex);
    EXPECT_EQ("mtx_lock_0400", ret, thrd_success);
    ret = mtx_unlock(&g_mtex);
    EXPECT_EQ("mtx_lock_0400", ret, thrd_success);
    return 0;
}

int threadfuncE(void *arg)
{
    int32_t ret = 0;
    ret = mtx_lock(&g_mtex);
    ret = mtx_lock(&g_mtex);
    EXPECT_EQ("mtx_lock_0500", ret, thrd_success);
    ret = mtx_unlock(&g_mtex);
    EXPECT_EQ("mtx_lock_0500", ret, thrd_success);
    return 0;
}

/**
 * @tc.name      : mtx_lock_0100
 * @tc.desc      : Verify process mtx_lock success when mtx_init second args is mtx_plain.
 * @tc.level     : Level 0
 */
void mtx_lock_0100(void)
{
    int result = mtx_init(&g_mtex, mtx_plain);
    if (result != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    thrd_t tid1;
    thrd_create(&tid1, threadfuncA, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
        return;
    }
    thrd_join(tid1, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
        return;
    }
}

/**
 * @tc.name      : mtx_lock_0200
 * @tc.desc      : Verify process mtx_lock success when mtx_init second args is mtx_timed.
 * @tc.level     : Level 0
 */
void mtx_lock_0200(void)
{
    int result = mtx_init(&g_mtex, mtx_timed);
    if (result != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    thrd_t tid2;
    thrd_create(&tid2, threadfuncB, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
        return;
    }
    thrd_join(tid2, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
        return;
    }
}

/**
 * @tc.name      : mtx_lock_0300
 * @tc.desc      : Verify process mtx_lock success when mtx_plain second args is mtx_plain | mtx_recursive.
 * @tc.level     : Level 0
 */
void mtx_lock_0300(void)
{
    int result = mtx_init(&g_mtex, mtx_plain | mtx_recursive);
    if (result != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    thrd_t tid3;
    thrd_create(&tid3, threadfuncC, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
        return;
    }
    thrd_join(tid3, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
        return;
    }
}

/**
 * @tc.name      : mtx_lock_0400
 * @tc.desc      : Verify process mtx_lock success when mtx_plain second args is mtx_timed | mtx_recursive.
 * @tc.level     : Level 0
 */
void mtx_lock_0400(void)
{
    int result= mtx_init(&g_mtex, mtx_timed | mtx_recursive);
    if (result != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    thrd_t tid4;
    thrd_create(&tid4, threadfuncD, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
        return;
    }
    thrd_join(tid4, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
        return;
    }
}


/**
 * @tc.name      : mtx_lock_0500
 * @tc.desc      : Verify process mtx_lock twice success when mtx_plain second args is mtx_plain | mtx_recursive.
 * @tc.level     : Level 1
 */
void mtx_lock_0500(void)
{
    int result = mtx_init(&g_mtex, mtx_plain | mtx_recursive);
    if (result != thrd_success) {
        t_error("%s mtx_init failed", __func__);
        return;
    }
    thrd_t tid5;
    thrd_create(&tid5, threadfuncE, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
        return;
    }
    thrd_join(tid5, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
        return;
    }
}

int main(void)
{
    mtx_lock_0100();
    mtx_lock_0200();
    mtx_lock_0300();
    mtx_lock_0400();
    mtx_lock_0500();
    return t_status;
}
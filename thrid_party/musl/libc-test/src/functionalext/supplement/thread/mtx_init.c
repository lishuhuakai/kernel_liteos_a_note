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
#include <unistd.h>
#include <threads.h>
#include "functionalext.h"

static mtx_t g_mtx;

/**
 * @tc.name      : mtx_init.
 * @tc.desc      : Verify mtx_init process success when second args is mtx_plain.
 * @tc.desc      : level 0
 */
void mtx_init_0100(void)
{
    int32_t ret = 0;
    ret = mtx_init(&g_mtx, mtx_plain);
    EXPECT_EQ("mtx_init_0100", ret, thrd_success);
}

/**
 * @tc.name      : mtx_init.
 * @tc.desc      : Verify mtx_init process success when second args is mtx_timed.
 * @tc.desc      : level 1
 */
void mtx_init_0200(void)
{
    int32_t ret = 0;
    ret = mtx_init(&g_mtx, mtx_timed);
    EXPECT_EQ("mtx_init_0200", ret, thrd_success);
}

/**
 * @tc.name      : mtx_init.
 * @tc.desc      : Verify mtx_init process success when second args is mtx_plain | mtx_recursive.
 * @tc.desc      : level 0
 */
void mtx_init_0300(void)
{
    int32_t ret = 0;
    ret = mtx_init(&g_mtx, mtx_plain | mtx_recursive);
    EXPECT_EQ("mtx_init_0300", ret, thrd_success);
}

/**
 * @tc.name      : mtx_init.
 * @tc.desc      : Verify mtx_init process success when second args is mtx_timed | mtx_recursive.
 * @tc.desc      : level 0
 */
void mtx_init_0400(void)
{
    int32_t ret = 0;
    ret = mtx_init(&g_mtx, mtx_timed | mtx_recursive);
    EXPECT_EQ("mtx_init_0400", ret, thrd_success);
}

int main(void)
{
    mtx_init_0100();
    mtx_init_0200();
    mtx_init_0300();
    mtx_init_0400();
    return t_status;
}

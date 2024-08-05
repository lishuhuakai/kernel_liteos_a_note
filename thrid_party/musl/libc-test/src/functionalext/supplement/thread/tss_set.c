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

#include <threads.h>
#include "test.h"

#define TSS_SET_VALUE (void *)0xFF
static tss_t key;

int tssfunc(void *arg)
{
    int ret;
    ret = tss_create(&key, NULL);
    if (ret != thrd_success) {
        t_error("%s tss_create failed", __func__);
    }
    ret = tss_set(key, TSS_SET_VALUE);
    if (ret != thrd_success) {
        t_error("%s tss_set failed", __func__);
    }
    void *value = tss_get(key);
    if (value == 0) {
        t_error("%s tss_get failed", __func__);
    }
    if (value != TSS_SET_VALUE) {
        t_error("%s tss_get return val is %p, expect %p", __func__, value, TSS_SET_VALUE);
    }

    thrd_exit(thrd_success);
}

/**
 * @tc.name      : tss_set_0100
 * @tc.desc      : Sets the value of the thread-specific storage identified by tss_id for the current thread to val
 * @tc.level     : Level 0
 */
void tss_set_0100(void)
{
    thrd_t id;
    int result;

    result = tss_create(&key, NULL);
    if (result != thrd_success) {
        t_error("%s tss_create failed", __func__);
    }

    result = thrd_create(&id, tssfunc, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_join(id, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    void *value = tss_get(key);
    if (value != 0) {
        t_error("%s it should have failed", __func__);
    }

    tss_delete(key);
}

int main(int argc, char *argv[])
{
    tss_set_0100();
    return t_status;
}
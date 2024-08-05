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

#include <signal.h>
#include <stdlib.h>
#include <threads.h>
#include "test.h"

static int count = 0;
const char *src = "Hello";

void exception_handler(int sig)
{
    exit(t_status);
}

int threadfunc(void *arg)
{
    signal(SIGSEGV, exception_handler);

    count++;
    thrd_exit(thrd_success);
}

/**
 * @tc.name      : thrd_create_0100
 * @tc.desc      : Creates a new thread executing the function func
 * @tc.level     : Level 0
 */
void thrd_create_0100(void)
{
    thrd_t id;
    int result;
    result = thrd_create(&id, threadfunc, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    if (result != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    result = thrd_join(id, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    if (count != 1) {
        t_error("%s failed, count is %d", __func__, count);
    }

    count = 0;
}

/**
 * @tc.name      : thrd_create_0200
 * @tc.desc      : Creates a new thread executing the function func(arg)
 * @tc.level     : Level 1
 */
void thrd_create_0200(void)
{
    thrd_t id;
    int result;
    result = thrd_create(&id, threadfunc, (char *)src);
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
    }

    result = thrd_sleep(&(struct timespec){.tv_sec = 1}, NULL);
    if (result != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    result = thrd_join(id, NULL);
    if (result != thrd_success) {
        t_error("%s thrd_join failed", __func__);
    }

    if (count != 1) {
        t_error("%s failed, count is %d", __func__, count);
    }
    count = 0;
}

/**
 * @tc.name      : thrd_create_0300
 * @tc.desc      : exception test
 * @tc.level     : Level 2
 */
void thrd_create_0300(void)
{
    thrd_create(NULL, threadfunc, NULL);
}

int main(int argc, char *argv[])
{
    thrd_create_0100();
    thrd_create_0200();
    thrd_create_0300();
    return t_status;
}
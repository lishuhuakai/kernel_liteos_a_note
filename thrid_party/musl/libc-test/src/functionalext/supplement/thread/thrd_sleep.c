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

extern int __thrd_sleep_time64(const struct timespec *, struct timespec *);

static int count = 0;

void exception_handler(int sig)
{
    exit(t_status);
}

int threadfunc(void *arg)
{
    count++;

    struct timespec const *tl = (struct timespec const *)arg;
    if (thrd_sleep(tl, NULL) != 0) {
        t_error("%s thrd_sleep failed", __func__);
    }

    thrd_exit(thrd_success);
}

/**
 * @tc.name      : thrd_sleep_0100
 * @tc.desc      : Thread waits for 1s
 * @tc.level     : Level 0
 */
void thrd_sleep_0100(void)
{
    int result;
    thrd_t id;
    struct timespec ts = {.tv_sec = 1};

    result = thrd_create(&id, threadfunc, (void *)(&ts));
    if (result != thrd_success) {
        t_error("%s thrd_create failed", __func__);
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
 * @tc.name      : thrd_sleep_0200
 * @tc.desc      : Negative value
 * @tc.level     : Level 1
 */
void thrd_sleep_0200(void)
{
    int result = thrd_sleep(&(struct timespec){.tv_sec = -1}, NULL);
    if (result != -2) {
        t_error("%s negative value", __func__);
    }
}

/**
 * @tc.name      : thrd_sleep_0300
 * @tc.desc      : Invalid parameter test
 * @tc.level     : Level 2
 */
void thrd_sleep_0300(void)
{
    signal(SIGSEGV, exception_handler);

    thrd_sleep(NULL, NULL);
}

/**
 * @tc.name      : thrd_sleep_time64_0200
 * @tc.desc      : Negative value
 * @tc.level     : Level 1
 */
void thrd_sleep_time64_0200(void)
{
    int result = __thrd_sleep_time64(&(struct timespec){.tv_sec = -1}, NULL);
    if (result != -2) {
        t_error("%s negative value", __func__);
    }
}

int main(int argc, char *argv[])
{
    thrd_sleep_0100();
    thrd_sleep_0200();
    thrd_sleep_0300();
    thrd_sleep_time64_0200();
    return t_status;
}
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

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "test.h"
#define SIGNUM 40
static int count = 0;
static timer_t timerid;

void handler(int sig)
{
    count++;
    usleep(1000000);
    int ret = timer_getoverrun(timerid);
    if (ret < 0) {
        t_error("%s timer_getoverrun failed, ret is %d\n", __func__, ret);
    }
    return;
}

/**
 * @tc.name      : timer_getoverrun_0100
 * @tc.desc      : Get overrun count for a per-process timer
 * @tc.level     : Level 0
 */
void timer_getoverrun_0100(void)
{
    struct sigevent sev;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGNUM;
    sev.sigev_value.sival_ptr = &timerid;

    signal(SIGNUM, handler);

    int result = timer_create(CLOCK_REALTIME, &sev, &timerid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    struct itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 1000000;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;

    result = timer_settime(timerid, 0, &its, NULL);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    while (count <= 0) {
        sleep(1);
    }
    result = timer_delete(timerid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : timer_getoverrun_0200
 * @tc.desc      : Invalid timerid
 * @tc.level     : Level 2
 */
void timer_getoverrun_0200(void)
{
    int result = timer_getoverrun(NULL);
    if (result != -1) {
        t_error("%s timer_getoverrun failed, ret is %d\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    timer_getoverrun_0100();
    timer_getoverrun_0200();

    return t_status;
}

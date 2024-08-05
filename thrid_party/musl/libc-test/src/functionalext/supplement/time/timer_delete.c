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

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include "test.h"

#define MAX (3)
static int count = 0;

void timerHandler(int sig)
{
    if (count < MAX) {
        count++;
    }
    printf("timer handler return\n");
    return;
}

/**
 * @tc.name      : timer_delete_0100
 * @tc.desc      : Create a timer with a timeout of 1s and delete it
 * @tc.level     : Level 0
 */
void timer_delete_0100(void)
{
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGUSR1;
    sev.sigev_value.sival_ptr = &timerid;
    signal(SIGUSR1, timerHandler);

    if (timer_create(CLOCK_MONOTONIC, &sev, &timerid) == -1) {
        t_error("%s timer create failed", __func__);
        return;
    }

    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        t_error("%s timer set time failed", __func__);
        return;
    }

    while (count != MAX) {
        sleep(1);
    }

    if (timer_delete(timerid) == -1 && errno == EINVAL) {
        t_error("%s timer delete failed", __func__);
    }
}

int main(int argc, char *argv[])
{
    timer_delete_0100();
    return t_status;
}
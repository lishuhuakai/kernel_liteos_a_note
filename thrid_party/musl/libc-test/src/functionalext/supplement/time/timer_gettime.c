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
#include <stdlib.h>
#include <time.h>
#include "test.h"

#define Nanoseconds (1000000000)
static int count = 0;

extern int __timer_gettime64(timer_t, struct itimerspec *);

void timerHandler(int sig)
{
    count++;
    return;
}

void exception_handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : timer_gettime_0100
 * @tc.desc      : Get the remaining time of the timer
 * @tc.level     : Level 0
 */
void timer_gettime_0100(void)
{
    timer_t timerid;
    struct sigevent sev;
    struct itimerspec its;
    struct itimerspec tmp;

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
    its.it_interval.tv_sec = 1;
    its.it_interval.tv_nsec = 0;

    if (timer_settime(timerid, 0, &its, NULL) == -1) {
        t_error("%s timer set time failed", __func__);
        return;
    }

    if (timer_gettime(timerid, &tmp) == -1 && errno == EINVAL) {
        t_error("%s timer gettimer failed", __func__);
    }

    if (tmp.it_value.tv_sec >= 1 || tmp.it_value.tv_nsec >= Nanoseconds) {
        t_error("%s get time failed", __func__);
    }

    while (count <= 0) {
        sleep(1);
    }

    if (timer_delete(timerid) != 0) {
        t_error("%s timer_delete failed", __func__);
    }
}

/**
 * @tc.name      : timer_gettime_0200
 * @tc.desc      : The return value of the function when the parameter is abnormal
 * @tc.level     : Level 2
 */
void timer_gettime_0200(void)
{
    int result = timer_gettime(NULL, NULL);
    if (result != -1 && errno != EINVAL) {
        t_error("%s failed result = %d", __func__, result);
    }
}

/**
 * @tc.name      : timer_gettime64_0200
 * @tc.desc      : The return value of the function when the parameter is abnormal
 * @tc.level     : Level 2
 */
void timer_gettime64_0200(void)
{
    int result = __timer_gettime64(NULL, NULL);
    if (result != -1 && errno != EINVAL) {
        t_error("%s failed result = %d", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    timer_gettime_0100();
    timer_gettime_0200();
    timer_gettime64_0200();
    return t_status;
}
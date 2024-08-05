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
#include <time.h>
#include <unistd.h>

#include "test.h"
#define SIGNUM 40
static int count = 0;

void handler(int sig)
{
    count++;

    return;
}

/**
 * @tc.name      : timer_create_0100
 * @tc.desc      : create a per-process timer
 * @tc.level     : Level 0
 */
void timer_create_0100(void)
{
    struct sigevent sev;
    timer_t timerid;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGNUM;
    sev.sigev_value.sival_ptr = &timerid;

    signal(SIGNUM, handler);

    int result = timer_create(CLOCK_REALTIME, &sev, &timerid);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    struct itimerspec its;
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

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
 * @tc.name      : timer_create_0200
 * @tc.desc      : create a per-process timer with invalid parameters
 * @tc.level     : Level 2
 */
void timer_create_0200(void)
{
    errno = 0;
    int result = timer_create(-1, NULL, NULL);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != EINVAL) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    timer_create_0100();
    timer_create_0200();

    return t_status;
}

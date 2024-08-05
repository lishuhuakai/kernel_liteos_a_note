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
#include <stdlib.h>
#include <sys/timerfd.h>
#include <time.h>
#include "test.h"

#define NANOSECOND (1000000000)
#define MICROSECONDS (1000000)

extern int __timerfd_gettime64(int, struct itimerspec *);

/**
 * @tc.name      : timerfd_gettime_0100
 * @tc.desc      : Get the time left by the timer until the next supermarket
 * @tc.level     : Level 0
 */
void timerfd_gettime_0100(void)
{
    struct itimerspec its = {{0, 0}, {2, 0}};
    struct itimerspec val;
    int fd, result;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd < 0) {
        t_error("%s timerfd_create failed", __func__);
        return;
    }

    result = timerfd_settime(fd, 0, &its, NULL);
    if (result != 0) {
        t_error("%s timerfd_settime failed", __func__);
        return;
    }

    result = usleep(MICROSECONDS);
    if (result != 0) {
        t_error("%s usleep failed", __func__);
        return;
    }

    result = timerfd_gettime(fd, &val);
    if (result != 0) {
        t_error("%s timerfd_gettime failed", __func__);
        return;
    }
    if (val.it_value.tv_nsec > NANOSECOND) {
        t_error("%s timerfd error");
    }
}

/**
 * @tc.name      : timerfd_gettime_0200
 * @tc.desc      : arms the timerfd with invalid parameters
 * @tc.level     : Level 2
 */
void timerfd_gettime_0200(void)
{
    int result = timerfd_gettime(-1, NULL);
    if (result != -1) {
        t_error("%s failed, result is %d", __func__, result);
    }
}

/**
 * @tc.name      : timerfd_gettime64_0100
 * @tc.desc      : Get the time left by the timer until the next supermarket
 * @tc.level     : Level 0
 */
void timerfd_gettime64_0100(void)
{
    struct itimerspec its = {{0, 0}, {2, 0}};
    struct itimerspec val;
    int fd, result;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd < 0) {
        t_error("%s timerfd_create failed", __func__);
        return;
    }

    result = timerfd_settime(fd, 0, &its, NULL);
    if (result != 0) {
        t_error("%s timerfd_settime failed", __func__);
        return;
    }

    result = usleep(MICROSECONDS);
    if (result != 0) {
        t_error("%s usleep failed", __func__);
        return;
    }

    result = __timerfd_gettime64(fd, &val);
    if (result != 0) {
        t_error("%s __timerfd_gettime64 failed", __func__);
        return;
    }
    if (val.it_value.tv_nsec > NANOSECOND) {
        t_error("%s timerfd error");
    }
}

int main(int argc, char *argv[])
{
    timerfd_gettime_0100();
    timerfd_gettime_0200();
    timerfd_gettime64_0100();
    return t_status;
}
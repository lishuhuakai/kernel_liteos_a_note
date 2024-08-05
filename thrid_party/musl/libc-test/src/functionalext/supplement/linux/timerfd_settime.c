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
#include <sys/timerfd.h>
#include <time.h>
#include "test.h"

#define NANOSECOND (1000000000)
#define MICROSECONDS (1000000)
#define DATA_TWO (2)

extern int __timerfd_settime64(int, int, const struct itimerspec *, struct itimerspec *);

void exception_handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : timerfd_settime_0100
 * @tc.desc      : Start the timer specified by fd
 * @tc.level     : Level 0
 */
void timerfd_settime_0100(void)
{
    struct itimerspec its = {{0, 0}, {DATA_TWO, 0}};
    struct itimerspec val;
    int fd, result;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd < 0) {
        t_error("%s timerfd_create failed\n", __func__);
        return;
    }

    result = timerfd_settime(fd, 0, &its, NULL);
    if (result != 0) {
        t_error("%s timerfd_settime failed\n", __func__);
        return;
    }

    result = usleep(MICROSECONDS);
    if (result != 0) {
        t_error("%s usleep failed\n", __func__);
        return;
    }

    result = timerfd_gettime(fd, &val);
    if (result != 0) {
        t_error("%s timerfd_gettime failed\n", __func__);
        return;
    }
    if (val.it_value.tv_nsec > NANOSECOND) {
        t_error("%s timerfd error\n");
    }
}

/**
 * @tc.name      : timerfd_settime_0200
 * @tc.desc      : arms the timer with invalid parameters
 * @tc.level     : Level 2
 */
void timerfd_settime_0200(void)
{
    signal(SIGSEGV, exception_handler);

    timerfd_settime(-1, 0, NULL, NULL);
}

/**
 * @tc.name      : timerfd_settime64_0100
 * @tc.desc      : Start the timer specified by fd
 * @tc.level     : Level 0
 */
void timerfd_settime64_0100(void)
{
    struct itimerspec its = {{0, 0}, {DATA_TWO, 0}};
    struct itimerspec val;
    int fd, result;

    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd < 0) {
        t_error("%s timerfd_create failed\n", __func__);
        return;
    }

    result = __timerfd_settime64(fd, 0, &its, NULL);
    if (result != 0) {
        t_error("%s __timerfd_settime64 failed\n", __func__);
        return;
    }

    result = usleep(MICROSECONDS);
    if (result != 0) {
        t_error("%s usleep failed\n", __func__);
        return;
    }

    result = timerfd_gettime(fd, &val);
    if (result != 0) {
        t_error("%s timerfd_gettime failed\n", __func__);
        return;
    }
    if (val.it_value.tv_nsec > NANOSECOND) {
        t_error("%s timerfd error\n");
    }
}

int main(int argc, char *argv[])
{
    timerfd_settime_0100();
    timerfd_settime_0200();
    timerfd_settime64_0100();
    return t_status;
}
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
#include <sched.h>

#include "test.h"

extern int __sched_rr_get_interval_time64(pid_t, struct timespec *);

/**
 * @tc.name      : sched_rr_get_interval_0100
 * @tc.desc      : get the SCHED_RR interval for the named process
 * @tc.level     : Level 0
 */
void sched_rr_get_interval_0100(void)
{
    pid_t pid = getpid();
    struct timespec ts = {0};

    errno = 0;
    int result = sched_rr_get_interval(pid, &ts);
    if (result != 0 || errno != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    if (ts.tv_sec < 0 || ts.tv_nsec < 0) {
        t_error("%s failed: ts.tv_sec = %ld\n", __func__, ts.tv_sec);
        t_error("%s failed: ts.tv_nsec = %ld\n", __func__, ts.tv_nsec);
    }
}

/**
 * @tc.name      : sched_rr_get_interval_0200
 * @tc.desc      : get the SCHED_RR interval for an invalid process
 * @tc.level     : Level 2
 */
void sched_rr_get_interval_0200(void)
{
    pid_t pid = -1;
    struct timespec ts = {0};

    errno = 0;
    int result = sched_rr_get_interval(pid, &ts);
    if (result == 0 || errno == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : sched_rr_get_interval_time64_0100
 * @tc.desc      : get the SCHED_RR interval for the named process
 * @tc.level     : Level 0
 */
void sched_rr_get_interval_time64_0100(void)
{
    pid_t pid = getpid();
    struct timespec ts = {0};

    errno = 0;
    int result = __sched_rr_get_interval_time64(pid, &ts);
    if (result != 0 || errno != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    if (ts.tv_sec < 0 || ts.tv_nsec < 0) {
        t_error("%s failed: ts.tv_sec = %ld\n", __func__, ts.tv_sec);
        t_error("%s failed: ts.tv_nsec = %ld\n", __func__, ts.tv_nsec);
    }
}

int main(int argc, char *argv[])
{
    sched_rr_get_interval_0100();
    sched_rr_get_interval_0200();
    sched_rr_get_interval_time64_0100();
    return t_status;
}

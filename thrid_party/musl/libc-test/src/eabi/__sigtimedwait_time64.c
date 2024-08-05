/* Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include "test.h"

extern int __sigtimedwait_time64(const sigset_t *restrict mask, 
                                siginfo_t *restrict si, 
                                const struct timespec *restrict timeout);

/**
 * @tc.name      : sigtimedwait_0100
 * @tc.desc      : Test sigtimedwait method
 * @tc.level     : Level 1
 */
void sigtimedwait_0100(void)
{
    sigset_t set;
    pid_t pid;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);
    pid = fork();
    if( pid == -1) {
        exit(1);
    } else if( pid ) {
        sigset_t set2;
        siginfo_t siginfo;
        struct timespec timeout = {3, 0};
        int signal;
        sigemptyset(&set2);
        sigaddset(&set2, SIGCHLD);
        signal = sigtimedwait(&set2, &siginfo, &timeout);
        if (signal < 0) {
            t_error( "%s sigtimedwait error get result is %d\n", __func__, signal);
        }
    }
}

/**
 * @tc.name      : sigtimedwait_time64_0100
 * @tc.desc      : Test __sigtimedwait_time64 method
 * @tc.level     : Level 1
 */
void sigtimedwait_time64_0100(void)
{
    sigset_t set;
    pid_t pid;
    sigemptyset(&set);
    sigaddset(&set, SIGCHLD);
    sigprocmask(SIG_BLOCK, &set, NULL);
    pid = fork();
    if( pid == -1) {
        exit(1);
    } else if( pid ) {
        sigset_t set2;
        siginfo_t siginfo;
        struct timespec timeout = {3, 0};
        int signal;
        sigemptyset(&set2);
        sigaddset(&set2, SIGCHLD);
        signal = __sigtimedwait_time64(&set2, &siginfo, &timeout);
        if (signal < 0) {
            t_error( "%s __sigtimedwait_time64 error get result is %d\n", __func__, signal);
        }
    }
}

int main()
{
    sigtimedwait_0100();
    sigtimedwait_time64_0100();
    return t_status;
}
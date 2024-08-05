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

#include "test.h"

/**
 * @tc.name      : sigwaitinfo_0100
 * @tc.desc      : wait for queued signals
 * @tc.level     : Level 0
 */
void sigwaitinfo_0100(void)
{
    // block a signal
    sigset_t set;
    sigemptyset(&set);

    int sig = SIGALRM;
    sigaddset(&set, sig);
    int result = sigprocmask(SIG_BLOCK, &set, NULL);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    // raise a signal
    union sigval sigval = {.sival_int = 1};
    result = sigqueue(getpid(), sig, sigval);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    // get the pending signal.
    siginfo_t info;
    errno = 0;
    result = sigwaitinfo(&set, &info);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    if (result != sig) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (info.si_signo != sig) {
        t_error("%s failed: info.si_signo = %d\n", __func__, info.si_signo);
    }

    if (info.si_value.sival_int != sigval.sival_int) {
        t_error("%s failed: info.si_value.sival_int = %d\n", __func__, info.si_value.sival_int);
    }
}

int main(int argc, char *argv[])
{
    sigwaitinfo_0100();

    return t_status;
}

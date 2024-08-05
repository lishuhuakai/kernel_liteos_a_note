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

static int action_count = 0;

void action(int sig, siginfo_t *info, void *ctx)
{
    if (sig != SIGALRM) {
        t_error("%s failed: sig = %d\n", __func__, sig);
    }

    if (info->si_signo != SIGALRM) {
        t_error("%s failed: info->si_signo = %d\n", __func__, info->si_signo);
    }

    if (info->si_code != SI_QUEUE) {
        t_error("%s failed: info->si_code = %d\n", __func__, info->si_code);
    }

    if (info->si_value.sival_int != 1) {
        t_error("%s failed: info->si_value.sival_int = %d\n", __func__, info->si_value.sival_int);
    }

    action_count++;
}

/**
 * @tc.name      : sigqueue_0100
 * @tc.desc      : queue a signal
 * @tc.level     : Level 0
 */
void sigqueue_0100(void)
{
    action_count = 0;

    int sig = SIGALRM;
    int flags = SA_SIGINFO;
    struct sigaction act = {.sa_flags = flags, .sa_sigaction = action};
    struct sigaction old_act = {0};
    int result = sigaction(sig, &act, &old_act);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        return;
    }

    union sigval sigval = {.sival_int = 1};

    errno = 0;
    result = sigqueue(getpid(), sig, sigval);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    if (action_count != 1) {
        t_error("%s failed: action_count = %d\n", __func__, action_count);
    }
}

/**
 * @tc.name      : sigqueue_0200
 * @tc.desc      : queue a signal with an invalid sig
 * @tc.level     : Level 2
 */
void sigqueue_0200(void)
{
    int sig = 99999;
    union sigval sigval = {.sival_int = 1};

    errno = 0;
    int result = sigqueue(getpid(), sig, sigval);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    sigqueue_0100();
    sigqueue_0200();

    return t_status;
}

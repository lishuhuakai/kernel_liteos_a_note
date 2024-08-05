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

static int handler_count = 0;

void handler(int sig)
{
    handler_count++;
}

/**
 * @tc.name      : sigsuspend_0100
 * @tc.desc      : wait for a signal
 * @tc.level     : Level 0
 */
void sigsuspend_0100(void)
{
    sigset_t set;
    sigemptyset(&set);

    int sig = SIGALRM;
    sigaddset(&set, sig);
    sigprocmask(SIG_BLOCK, &set, NULL);

    int flags = 0;
    struct sigaction act = {.sa_flags = flags, .sa_handler = handler};
    struct sigaction old_act = {0};
    int result = sigaction(sig, &act, &old_act);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    sigset_t pending_set;
    sigemptyset(&pending_set);
    sigpending(&pending_set);

    for (int i = SIGHUP; i <= SIGSTKSZ; ++i) {
        result = sigismember(&pending_set, i);
        if (result != 0) {
            t_error("%s failed: result = %d, i = %d\n", __func__, result, i);
        }
    }

    raise(sig);
    if (handler_count != 0) {
        t_error("%s failed: handler_count = %d\n", __func__, handler_count);
    }

    sigemptyset(&pending_set);
    sigpending(&pending_set);

    for (int i = SIGHUP; i <= SIGSTKSZ; ++i) {
        if ((i == sig) != (sigismember(&pending_set, i))) {
            t_error("%s failed: i = %d, sig = %d\n", __func__, i, sig);
        }
    }

    sigset_t set_without_sig;
    sigfillset(&set_without_sig);
    sigdelset(&set_without_sig, sig);

    result = sigsuspend(&set_without_sig);
    if (result != -1) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (errno != EINTR) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    if (handler_count != 1) {
        t_error("%s failed: handler_count = %d\n", __func__, handler_count);
    }
}

int main(int argc, char *argv[])
{
    sigsuspend_0100();

    return t_status;
}

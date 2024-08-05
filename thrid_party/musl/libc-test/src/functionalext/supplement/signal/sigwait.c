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

#include "test.h"

/**
 * @tc.name      : sigwait_0100
 * @tc.desc      : wait for a signal
 * @tc.level     : Level 0
 */
void sigwait_0100(void)
{
    sigset_t set = {0};
    int result = sigemptyset(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    int sig = SIGALRM;
    result = sigaddset(&set, sig);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigprocmask(SIG_BLOCK, &set, NULL);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    union sigval sigval = {.sival_int = 1};
    result = sigqueue(getpid(), sig, sigval);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    sig = 0;
    result = sigwait(&set, &sig);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    if (sig != SIGALRM) {
        t_error("%s failed: sig = %d\n", __func__, sig);
    }
}

int main(int argc, char *argv[])
{
    sigwait_0100();

    return t_status;
}

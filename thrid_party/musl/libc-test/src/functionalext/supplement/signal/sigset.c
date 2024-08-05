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
const int sig = SIGALRM;

void handler(int sig)
{
    handler_count++;
}

int prepare(sigset_t *set)
{
    handler_count = 0;

    struct sigaction act = {.sa_flags = 0, .sa_handler = handler};
    struct sigaction old_act = {0};
    int result = sigaction(sig, &act, &old_act);
    if (result != 0) {
        return result;
    }

    if (handler_count != 0) {
        return -1;
    }

    result = raise(sig);
    if (result != 0) {
        return result;
    }

    if (handler_count != 1) {
        return -1;
    }

    result = sigemptyset(set);
    if (result != 0) {
        return result;
    }

    result = sigaddset(set, sig);
    if (result != 0) {
        return result;
    }

    result = sigprocmask(SIG_BLOCK, NULL, set);
    if (result != 0) {
        return result;
    }

    return 0;
}

/**
 * @tc.name      : sigset_0100
 * @tc.desc      : modifies the disposition of the signal sig with a handler
 * @tc.level     : Level 0
 */
void sigset_0100(void)
{
    sigset_t set;
    int result = prepare(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigemptyset(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    errno = 0;
    sigset(sig, handler);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    result = sigprocmask(SIG_BLOCK, NULL, &set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigismember(&set, sig);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : sigset_0200
 * @tc.desc      : modifies the disposition of the signal sig with SIG_IGN
 * @tc.level     : Level 1
 */
void sigset_0200(void)
{
    sigset_t set;
    int result = prepare(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigemptyset(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    errno = 0;
    sigset(sig, SIG_IGN);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    result = sigprocmask(SIG_BLOCK, NULL, &set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigismember(&set, sig);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : sigset_0300
 * @tc.desc      : modifies the disposition of the signal sig with SIG_DFL
 * @tc.level     : Level 1
 */
void sigset_0300(void)
{
    sigset_t set;
    int result = prepare(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigemptyset(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    errno = 0;
    sigset(sig, SIG_DFL);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    result = sigprocmask(SIG_BLOCK, NULL, &set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigismember(&set, sig);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : sigset_0400
 * @tc.desc      : modifies the disposition of the signal sig with SIG_HOLD
 * @tc.level     : Level 1
 */
void sigset_0400(void)
{
    sigset_t set;
    int result = prepare(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigemptyset(&set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    errno = 0;
    sigset(sig, SIG_HOLD);
    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    result = sigprocmask(SIG_BLOCK, NULL, &set);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigismember(&set, sig);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

/**
 * @tc.name      : sigset_0500
 * @tc.desc      : modifies the disposition of the invalid signal sig
 * @tc.level     : Level 2
 */
void sigset_0500(void)
{
    errno = 0;
    int sig = 99999;

    sigset(sig, SIG_DFL);
    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    sigset_0100();
    sigset_0200();
    sigset_0300();
    sigset_0400();
    sigset_0500();

    return t_status;
}

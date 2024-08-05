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
 * @tc.name      : sigignore_0100
 * @tc.desc      : sets the disposition of sig to SIG_IGN
 * @tc.level     : Level 0
 */
void sigignore_0100(void)
{
    int sig = SIGALRM;
    struct sigaction old_act = {0};
    errno = 0;
    int result = sigaction(sig, NULL, &old_act);
    if (result != 0 || errno != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    errno = 0;
    result = sigignore(sig);
    if (result != 0 || errno != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    struct sigaction sa;
    errno = 0;
    result = sigaction(SIGALRM, NULL, &sa);
    if (result != 0 || errno != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    if (SIG_IGN != sa.sa_handler) {
        t_error("%s failed: sa.sa_handler\n", __func__);
    }
}

/**
 * @tc.name      : sigignore_0200
 * @tc.desc      : sets the disposition of the SIGKILL sig to SIG_IGN
 * @tc.level     : Level 1
 */
void sigignore_0200(void)
{
    errno = 0;
    int result = sigignore(SIGKILL);
    if (result != -1 || errno != EINVAL) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : sigignore_0300
 * @tc.desc      : sets the disposition of the SIGSTOP sig to SIG_IGN
 * @tc.level     : Level 1
 */
void sigignore_0300(void)
{
    errno = 0;
    int result = sigignore(SIGSTOP);
    if (result != -1 || errno != EINVAL) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

/**
 * @tc.name      : sigignore_0400
 * @tc.desc      : sets the disposition of an invalid sig to SIG_IGN
 * @tc.level     : Level 2
 */
void sigignore_0400(void)
{
    int sig = 99999;

    errno = 0;
    int result = sigignore(sig);
    if (result == 0 || errno != EINVAL) {
        t_error("%s failed: result = %d\n", __func__, result);
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    sigignore_0100();
    sigignore_0200();
    sigignore_0300();
    sigignore_0400();

    return t_status;
}

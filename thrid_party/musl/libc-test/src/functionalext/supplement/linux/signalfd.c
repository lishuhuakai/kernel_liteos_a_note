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
#include <sys/signalfd.h>

#include "test.h"

const int sig = SIGALRM;

/**
 * @tc.name      : signalfd_0100
 * @tc.desc      : create a file descriptor for accepting signals
 * @tc.level     : Level 0
 */
void signalfd_0100(void)
{
    sigset_t mask = {};

    int result = sigaddset(&mask, sig);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    result = sigprocmask(SIG_SETMASK, &mask, NULL);
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    errno = 0;
    int fd = signalfd(-1, &mask, SFD_CLOEXEC);
    if (fd <= 0) {
        t_error("%s failed: fd = %d\n", __func__, fd);
    }

    if (errno != 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }

    raise(sig);

    struct signalfd_siginfo sfd_si = {0};
    ssize_t size = read(fd, &sfd_si, sizeof(sfd_si));
    if (size <= 0) {
        t_error("%s failed: size = %ld\n", __func__, size);
    }

    if (sfd_si.ssi_signo != sig) {
        t_error("%s failed: sfd_si.ssi_signo = %d\n", __func__, sfd_si.ssi_signo);
    }

    close(fd);
}

/**
 * @tc.name      : signalfd_0200
 * @tc.desc      : create a file descriptor for accepting signals with invalid parameters
 * @tc.level     : Level 2
 */
void signalfd_0200(void)
{
    errno = 0;
    int fd = signalfd(-1, NULL, -1);
    if (fd > 0) {
        t_error("%s failed: fd = %d\n", __func__, fd);
    }

    if (errno == 0) {
        t_error("%s failed: errno = %d\n", __func__, errno);
    }
}

int main(int argc, char *argv[])
{
    signalfd_0100();
    signalfd_0200();

    return t_status;
}

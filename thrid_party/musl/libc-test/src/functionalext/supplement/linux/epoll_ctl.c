/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Solve the build error of this test code when importing the header file <sys/epoll.h>
#ifndef _EPOLL_CTL_TEST
#define _EPOLL_CTL_TEST
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#undef _GNU_SOURCE
#undef _BSD_SOURCE
#endif
#endif

#include <errno.h>
#include <sys/epoll.h>
#include "test.h"

/**
 * @tc.name      : epoll_ctl_0100
 * @tc.desc      : Add entries in the interest list of the epoll instance referred to by the file descriptor.
 * @tc.level     : Level 0
 */
void epoll_ctl_0100(void)
{
    errno = 0;
    int fds[2];
    const uint64_t expected = 0x123456789abcdef0;

    int fd = epoll_create(1);
    if (fd == -1) {
        t_error("%s epoll_create failed\n", __func__);
    }
    if (errno != 0) {
        t_error("%s errno is %d\n", __func__, errno);
    }

    int ret = pipe(fds);
    if (ret == -1) {
        t_error("%s pipe failed\n", __func__);
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.u64 = expected;

    int result = epoll_ctl(fd, EPOLL_CTL_ADD, fds[0], &ev);
    if (result == -1) {
        t_error("%s epoll_ctl failed\n", __func__);
    }

    ret = write(fds[1], "\n", 1);
    if (ret != 1) {
        t_error("%s nothing in pipe\n", __func__);
    }

    struct epoll_event events[1];
    result = epoll_wait(fd, events, 1, 1);
    if (events[0].data.u64 != expected) {
        t_error("%s events[0].data.u64 is not right\n", __func__);
    }

    close(fds[0]);
    close(fds[1]);
}

int main(int argc, char *argv[])
{
    epoll_ctl_0100();
    return t_status;
}

#ifdef _EPOLL_CTL_TEST
#undef _EPOLL_CTL_TEST
#endif
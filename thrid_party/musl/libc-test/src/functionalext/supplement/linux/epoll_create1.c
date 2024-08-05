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
#ifndef _EPOLL_CREATE1_TEST
#define _EPOLL_CREATE1_TEST
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#undef _GNU_SOURCE
#undef _BSD_SOURCE
#endif
#endif

#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/epoll.h>
#include "test.h"

void close_on_exec(int fd, bool close_on_exec)
{
    int flags = fcntl(fd, F_GETFD);
    if (flags == -1) {
        t_error("%s fcntl failed\n", __func__);
    }
    if ((close_on_exec ? FD_CLOEXEC : 0) != (flags & FD_CLOEXEC)) {
        t_error("%s failed, expect equal\n", __func__);
    }
}

/**
 * @tc.name      : epoll_create1_0100
 * @tc.desc      : Returns a file descriptor referring to the new epoll instance
 * @tc.level     : Level 0
 */
void epoll_create1_0100(void)
{
    errno = 0;

    int fd;
    fd = epoll_create1(0);
    close_on_exec(fd, false);
    close(fd);

    fd = epoll_create1(EPOLL_CLOEXEC);
    close_on_exec(fd, true);
    close(fd);
}

int main(int argc, char *argv[])
{
    epoll_create1_0100();
    return t_status;
}

#ifdef _EPOLL_CREATE1_TEST
#undef _EPOLL_CREATE1_TEST
#endif
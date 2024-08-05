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
#ifndef _EPOLL_WAIT_TEST
#define _EPOLL_WAIT_TEST
#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#undef _GNU_SOURCE
#undef _BSD_SOURCE
#endif
#endif

#include <errno.h>
#include <sys/epoll.h>
#include "test.h"

/**
 * @tc.name      : epoll_wait_0100
 * @tc.desc      : Regular epoll_wait
 * @tc.level     : Level 0
 */
void epoll_wait_0100(void)
{
    int epoll_fd = epoll_create(1);
    if (epoll_fd == -1) {
        t_error("%s epoll_create failed\n", __func__);
    }

    struct epoll_event events[1];
    int result = epoll_wait(epoll_fd, events, 1, 1);
    if (result != 0) {
        t_error("%s epoll_pwait failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    epoll_wait_0100();
    return t_status;
}

#ifdef _EPOLL_WAIT_TEST
#undef _EPOLL_WAIT_TEST
#endif
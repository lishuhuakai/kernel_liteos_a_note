/**
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

#include <sys/select.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "functionalext.h"

extern int __pselect_time64 (int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict, 
                    const struct timespec *__restrict, const sigset_t *__restrict);


/**
 * @tc.name      : pselect_fdset_0100
 * @tc.desc      : Determine whether the fdset macro is normal
 * @tc.level     : Level 0
 */
void pselect_fdset_0100(void)
{
    fd_set set;
    FD_ZERO(&set);
    for (size_t i = 0; i < 1024; ++i) {
        EXPECT_FALSE("pselect_fdset_0100", FD_ISSET(i, &set));
    }
    FD_SET(STDIN_FILENO, &set);
    EXPECT_TRUE("pselect_fdset_0100", FD_ISSET(STDIN_FILENO, &set));
    EXPECT_FALSE("pselect_fdset_0100", FD_ISSET(STDOUT_FILENO, &set));
    FD_SET(STDOUT_FILENO, &set);
    EXPECT_TRUE("pselect_fdset_0100", FD_ISSET(STDIN_FILENO, &set));
    EXPECT_TRUE("pselect_fdset_0100", FD_ISSET(STDOUT_FILENO, &set));
    FD_CLR(STDIN_FILENO, &set);
    EXPECT_FALSE("pselect_fdset_0100", FD_ISSET(STDIN_FILENO, &set));
    EXPECT_TRUE("pselect_fdset_0100", FD_ISSET(STDOUT_FILENO, &set));
    FD_CLR(STDOUT_FILENO, &set);
    EXPECT_FALSE("pselect_fdset_0100", FD_ISSET(STDIN_FILENO, &set));
    EXPECT_FALSE("pselect_fdset_0100", FD_ISSET(STDOUT_FILENO, &set));
}

/**
 * @tc.name      : pselect_normal_0100
 * @tc.desc      : Monitor stdout file for writability (normal)
 * @tc.level     : Level 0
 */
void pselect_normal_0100(void)
{
    fd_set set;
    struct timespec timeout;
    timeout.tv_sec = 5;
    timeout.tv_nsec = 0;
    FD_ZERO(&set);
    FD_SET(STDOUT_FILENO, &set);

    int ret = pselect(STDOUT_FILENO + 1, NULL, &set, NULL, &timeout, NULL);
    EXPECT_EQ("pselect_normal_0100", ret, 1);
    EXPECT_TRUE("pselect_normal_0100", FD_ISSET(STDOUT_FILENO, &set));
}

/**
 * @tc.name      : pselect_error_0100
 * @tc.desc      : Incoming illegal file descriptor size to monitor stdout file writability
 * @tc.level     : Level 2
 */
void pselect_error_0100(void)
{
    fd_set set;
    struct timespec timeout;
    timeout.tv_sec = 5;
    timeout.tv_nsec = 0;
    FD_ZERO(&set);
    FD_SET(STDOUT_FILENO, &set);

    int ret = pselect(-1, NULL, &set, NULL, &timeout, NULL);
    EXPECT_EQ("pselect_error_0100", ret, ERREXPECT);
}

/**
 * @tc.name      : pselect_timeout_0100
 * @tc.desc      : Monitor stdout file for readability (timeout)
 * @tc.level     : Level 2
 */
void pselect_timeout_0100(void)
{
    fd_set set;
    sigset_t sigmask;
    struct timespec timeout;
    timeout.tv_sec = 2;
    timeout.tv_nsec = 0;
    FD_ZERO(&set);
    FD_SET(STDOUT_FILENO, &set);
    sigemptyset(&sigmask);
    sigaddset(&sigmask, SIGINT);
    int ret = pselect(STDOUT_FILENO + 1, &set, NULL, NULL, &timeout, &sigmask);
    if (ret == 0) {
        EXPECT_FALSE("pselect_timeout_0100", FD_ISSET(STDOUT_FILENO, &set));
    }
}

/**
 * @tc.name      : pselect_time64_normal_0100
 * @tc.desc      : Monitor stdout file for writability (normal)
 * @tc.level     : Level 0
 */
void pselect_time64_normal_0100(void)
{
    fd_set set;
    struct timespec timeout;
    timeout.tv_sec = 5;
    timeout.tv_nsec = 0;
    FD_ZERO(&set);
    FD_SET(STDOUT_FILENO, &set);

    int ret = __pselect_time64(STDOUT_FILENO + 1, NULL, &set, NULL, &timeout, NULL);
    EXPECT_EQ("pselect_time64_normal_0100", ret, 1);
    EXPECT_TRUE("pselect_time64_normal_0100", FD_ISSET(STDOUT_FILENO, &set));
}

int main(void)
{
    pselect_fdset_0100();
    pselect_normal_0100();
    pselect_error_0100();
    pselect_timeout_0100();
    pselect_time64_normal_0100();

    return t_status;
}

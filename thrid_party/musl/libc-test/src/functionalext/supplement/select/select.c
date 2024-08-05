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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include "functionalext.h"

extern int __select_time64(int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict, struct timeval *__restrict);

const int32_t COUNT_ZERO = 0;
const int32_t COUNT_FAILED = -1;

/**
 * @tc.name      : select_0100
 * @tc.desc      : Verify that the monitoring descriptor is successful (valid parameters)
 * @tc.level     : Level 0
 */
void select_0100(void)
{
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(0, &readfds);
    FD_SET(1, &writefds);
    int result;
    result = select(2, &readfds, &writefds, 0, &timeout);
    EXPECT_TRUE("select_0100", result > 0);
}

/**
 * @tc.name      : select_0200
 * @tc.desc      : Failed to validate monitoring descriptor
 *                 (invalid parameter, microsecond invalid value in timeout structure)
 * @tc.level     : Level 2
 */
void select_0200(void)
{
    struct timeval timeout;
    timeout.tv_sec = -50;
    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(0, &readfds);
    FD_SET(1, &writefds);
    int result;
    result = select(2, &readfds, &writefds, 0, &timeout);
    EXPECT_EQ("select_0200", result, COUNT_FAILED);
}

/**
 * @tc.name      : select_0300
 * @tc.desc      : Validation monitoring descriptor failed
 *                 (invalid parameter, invalid parameter added to descriptor set)
 * @tc.level     : Level 1
 */
void select_0300(void)
{
    struct timeval timeout;
    timeout.tv_sec = 0;
    fd_set readfds, writefds;
    int fd1, fd2;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(2, &readfds);
    FD_SET(2, &writefds);
    int result;
    result = select(2, &readfds, &writefds, 0, &timeout);
    EXPECT_EQ("select_0300", result, COUNT_ZERO);
}

/**
 * @tc.name      : select_time64_0100
 * @tc.desc      : Verify that the monitoring descriptor is successful (valid parameters)
 * @tc.level     : Level 0
 */
void select_time64_0100(void)
{
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(0, &readfds);
    FD_SET(1, &writefds);
    int result;
    result = __select_time64(2, &readfds, &writefds, 0, &timeout);
    EXPECT_TRUE("select_time64_0100", result > 0);
}

int main(void)
{
    select_0100();
    select_0200();
    select_0300();
    select_time64_0100();
    return t_status;
}

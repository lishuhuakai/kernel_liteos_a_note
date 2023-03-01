/*
 * Copyright (c) 2023-2023 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <gtest/gtest.h>
#include "It_process_plimits.h"

void ItProcessPlimitsIpc003(void)
{
    mode_t mode;
    int ret;
    std::string plimitsPath = "/proc/plimits/test";
    std::string configFileMqCount = "/proc/plimits/test/ipc.mq_limit";
    std::string configFileShmSize = "/proc/plimits/test/ipc.shm_limit";
    std::string configFileStat = "/proc/plimits/test/ipc.stat";

    ret = mkdir(plimitsPath.c_str(), S_IFDIR | mode);
    ASSERT_EQ(ret, 0);

    int fd = access(configFileMqCount.c_str(), F_OK | W_OK | R_OK);
    ASSERT_EQ(fd, 0);

    fd = access(configFileShmSize.c_str(), F_OK | W_OK | R_OK);
    ASSERT_EQ(fd, 0);

    fd = access(configFileStat.c_str(), F_OK | R_OK);
    ASSERT_EQ(fd, 0);

    fd = access(configFileStat.c_str(), W_OK | X_OK);
    ASSERT_EQ(fd, -1);

    ret = rmdir(plimitsPath.c_str());
    ASSERT_EQ(ret, 0);
    return;
}

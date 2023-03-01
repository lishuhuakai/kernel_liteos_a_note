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

void ItProcessPlimitsIpc008(void)
{
    mode_t mode;
    int ret;
    std::string plimitsPath = "/proc/plimits/test";
    std::string configFileMqCount = "/proc/plimits/test/ipc.mq_limit";
    std::string mqLimitCount_0 = "111*";
    std::string mqLimitCount_1 = "123abc";
    std::string mqLimitCount_2 = "\"123    456\"";
    std::string mqLimitCount_3 = "\123";
    std::string mqLimitCount_4 = "10000000000000000000000000000000001";
    std::string mqLimitCount_5 = "\1\2\3";

    ret = mkdir(plimitsPath.c_str(), S_IFDIR | mode);
    ASSERT_EQ(ret, 0);

    ret = WriteFile(configFileMqCount.c_str(), mqLimitCount_0.c_str());
    ASSERT_EQ(ret, -1);

    ret = WriteFile(configFileMqCount.c_str(), mqLimitCount_1.c_str());
    ASSERT_EQ(ret, -1);

    ret = WriteFile(configFileMqCount.c_str(), mqLimitCount_2.c_str());
    ASSERT_EQ(ret, -1);

    ret = WriteFile(configFileMqCount.c_str(), mqLimitCount_3.c_str());
    ASSERT_EQ(ret, -1);

    ret = WriteFile(configFileMqCount.c_str(), mqLimitCount_4.c_str());
    ASSERT_EQ(ret, -1);

    ret = WriteFile(configFileMqCount.c_str(), mqLimitCount_5.c_str());
    ASSERT_EQ(ret, -1);

    ret = rmdir(plimitsPath.c_str());
    ASSERT_EQ(ret, 0);
    return;
}

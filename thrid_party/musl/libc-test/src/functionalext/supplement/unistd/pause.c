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

#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "functionalext.h"

#define TEST_ALARM_TIME 2

int gtest_pause_flag = 0;
static void signal_func(int signum)
{
    if (signum == SIGALRM) {
        gtest_pause_flag = 1;
    }
}

/**
 * @tc.name      : pause_0100
 * @tc.desc      : Wake up a suspended process with a signal
 * @tc.level     : Level 0
 */
void pause_0100(void)
{
    struct sigaction act;
    act.sa_handler = signal_func;
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);

    alarm(TEST_ALARM_TIME);
    EXPECT_FALSE("pause_0100", gtest_pause_flag);
    int ret = pause();
    EXPECT_EQ("pause_0100", ret, ERREXPECT);
    EXPECT_EQ("pause_0100", errno, EINTR);
    EXPECT_TRUE("pause_0100", gtest_pause_flag);
}

int main(void)
{
    // pause_0100();
    return t_status;
}
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

#include <signal.h>
#include <string.h>
#include "functionalext.h"

/**
 * @tc.name      : psiginfo_0100
 * @tc.desc      : Pass the normal parameters, the function call is successful,
 *                 and the message is output in the console
 * @tc.level     : Level 0
 */
void psiginfo_0100(void)
{
    errno = 0;
    siginfo_t info;
    memset(&info, 0x0, sizeof(siginfo_t));
    info.si_signo = SIGUSR1;
    psiginfo(&info, "SIGUSR1");
    EXPECT_EQ("psiginfo_0100", errno, CMPFLAG);
}

/**
 * @tc.name      : psiginfo_0200
 * @tc.desc      : Invalid parameters are passed in, the function call succeeds,
 *                 and a message is output to the console
 * @tc.level     : Level 0
 */
void psiginfo_0200(void)
{
    errno = 0;
    siginfo_t info;
    memset(&info, 0x0, sizeof(siginfo_t));
    info.si_signo = SIGUSR1;
    psiginfo(&info, NULL);
    EXPECT_EQ("psiginfo_0200", errno, CMPFLAG);
}

/**
 * @tc.name      : psiginfo_0300
 * @tc.desc      : Invalid parameters are passed in, the function call succeeds,
 *                 and a message is output to the console
 * @tc.level     : Level 2
 */
void psiginfo_0300(void)
{
    errno = 0;
    siginfo_t info;
    memset(&info, 0x0, sizeof(siginfo_t));
    info.si_signo = -1;
    psiginfo(&info, NULL);
    EXPECT_EQ("psiginfo_0300", errno, CMPFLAG);
}

int main(void)
{
    psiginfo_0100();
    psiginfo_0200();
    psiginfo_0300();
    return t_status;
}
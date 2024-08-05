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
 * @tc.name      : psignal_0100
 * @tc.desc      : Pass the normal parameters, the function call is successful,
 *                 and the message is output in the console
 * @tc.level     : Level 0
 */
void psignal_0100(void)
{
    errno = 0;
    psignal(SIGUSR1, "SIGUSR1");
    EXPECT_EQ("psignal_0100", errno, CMPFLAG);
}

/**
 * @tc.name      : psignal_0200
 * @tc.desc      : Invalid parameters are passed in, the function call succeeds,
 *                 and a message is output to the console
 * @tc.level     : Level 0
 */
void psignal_0200(void)
{
    errno = 0;
    psignal(SIGUSR1, NULL);
    EXPECT_EQ("psignal_0200", errno, CMPFLAG);
}

/**
 * @tc.name      : psignal_0300
 * @tc.desc      : Invalid parameters are passed in, the function call succeeds,
 *                 and a message is output to the console
 * @tc.level     : Level 2
 */
void psignal_0300(void)
{
    errno = 0;
    psignal(-1, NULL);
    EXPECT_EQ("psignal_0300", errno, CMPFLAG);
}

int main(void)
{
    psignal_0100();
    psignal_0200();
    psignal_0300();
    return t_status;
}
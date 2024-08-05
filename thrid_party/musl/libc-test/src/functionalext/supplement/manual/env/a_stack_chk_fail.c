/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <signal.h>
#include <stdlib.h>
#include "functionalext.h"

extern void __stack_chk_fail(void);

const int32_t INIT_LEN = 0;
const int32_t INCREASE_LEN = 1;
int32_t flag = 0;

void Killfunc(int signu)
{
    switch (signu) {
        case SIGILL:
            flag++;
            EXPECT_EQ("a_stack_chk_fail_0100 exception", flag, INCREASE_LEN);
            exit(0);
        case SIGSEGV:
            flag++;
            EXPECT_EQ("a_stack_chk_fail_0100 exception", flag, INCREASE_LEN);
            exit(0);
        default:
            break;
    }
    return;
}

/**
 * @tc.name      : a_stack_chk_fail_0100
 * @tc.desc      : Verify stack check fail process success
 * @tc.level     : Level 0
 */
void a_stack_chk_fail_0100(void)
{
    struct sigaction act, oldact;
    act.sa_handler = Killfunc;
    act.sa_flags = 0;
    sigaction(SIGILL, &act, &oldact);
    sigaction(SIGSEGV, &act, &oldact);
    __stack_chk_fail();
    EXPECT_EQ("a_stack_chk_fail_0100", flag, INCREASE_LEN);
}

int main(void)
{
    a_stack_chk_fail_0100();
    return t_status;
}

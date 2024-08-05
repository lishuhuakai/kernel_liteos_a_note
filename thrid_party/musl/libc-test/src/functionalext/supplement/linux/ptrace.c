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

#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"
#include "sys/ptrace.h"

#define PTRACE_TRACEME 0

/**
 * @tc.name      : ptrace_0100
 * @tc.desc      : Verify ptrace process success
 * @tc.level     : Level 1
 */
void ptrace_0100(void)
{
    int32_t ret;
    pid_t child_pid = fork();
    ret = ptrace(PTRACE_TRACEME);
    EXPECT_EQ("realpath_0100", ret, 0);
}

int main(void)
{
    ptrace_0100();
    return t_status;
}

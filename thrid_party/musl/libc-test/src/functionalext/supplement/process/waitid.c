/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
#include <sys/wait.h>
#include "test.h"

/**
 * @tc.name      : waitid_0100
 * @tc.desc      : Test waitid function for process control
 * @tc.level     : Level 0
 */
void waitid_0100(void)
{
    pid_t pid = fork();
    if (pid == -1) {
        t_error("%s waitid fork error\n", __func__);
    }
    if (pid == 0)
        _exit(66);
    siginfo_t si = {};
    int result = waitid(P_PID, pid, &si, WEXITED);
    if (result != 0) {
        t_error("%s waitid error get result is %d are not want 0\n", __func__, result);
    }
    if (pid != si.si_pid) {
        t_error("%s waitid error pid not = si.si_pid\n", __func__);
    }
    if (si.si_status != 66) {
        t_error("%s waitid error si.si_status not = 66\n", __func__);
    }
    if (si.si_code != CLD_EXITED) {
        t_error("%s waitid error si.si_code not = CLD_EXITED\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    waitid_0100();
    return t_status;
}
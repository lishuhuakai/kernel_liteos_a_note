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

#include <sys/wait.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getppid_0100
 * @tc.desc      : Get the parent process id of the calling process
 * @tc.level     : Level 0
 */
void getppid_0100(void)
{
    pid_t ppid = getpid();
    pid_t pid = fork();
    if (pid == 0) {
        pid_t child_ppid = getppid();
        EXPECT_EQ("getppid_0100", child_ppid, ppid);
        _exit(0);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        EXPECT_FALSE("getppid_0100", 1);
    }
}

int main(void)
{
    getppid_0100();
    return t_status;
}
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
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "functionalext.h"

/**
 * @tc.name      : quick_exit_0100
 * @tc.desc      : The main thread creates a child thread, the child thread calls quick exit to exit,
 *                 and the child thread exit status is received in the child thread
 * @tc.level     : Level 0
 */
void quick_exit_0100(void)
{
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        sleep(1);
        quick_exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        wait(&status);
        if (WIFEXITED(status)) {
            EXPECT_EQ("quick_exit_0100", EXIT_SUCCESS, WEXITSTATUS(status));
        }
    } else {
        t_error("Fork wrong\n");
        exit(EXIT_FAILURE);
    }
}

int main(void)
{
    quick_exit_0100();

    return t_status;
}

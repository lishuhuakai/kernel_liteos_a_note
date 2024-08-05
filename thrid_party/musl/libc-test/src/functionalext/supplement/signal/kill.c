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

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

static int exit_flag = 0;

void Killfunc(int signal)
{
    switch (signal) {
        case SIGUSR1:
            exit_flag = 1;
            break;
        default:
            break;
    }
    return;
}

/**
 * @tc.name      : kill_0100
 * @tc.desc      : Determine whether the child process successfully receives the signal to
 *                 the main process and respond accordingly
 * @tc.level     : Level 1
 */
void kill_0100(void)
{
    int status;
    pid_t pid = fork();
    if (pid == 0) {
        sleep(1);
        kill(getppid(), SIGUSR1);
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        signal(SIGUSR1, Killfunc);

        while (!exit_flag) {
            sleep(1);
        }
        wait(NULL);
        EXPECT_EQ("kill_0100", exit_flag, 1);
    } else {
        printf("Fork wrong\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @tc.name      : kill_0200
 * @tc.desc      : Outlier judgment
 * @tc.level     : Level 2
 */
void kill_0200(void)
{
    int rev = kill(0, 1000);
    EXPECT_EQ("kill_0200", rev, ERREXPECT);
}

int main(void)
{
    kill_0100();
    kill_0200();

    return t_status;
}
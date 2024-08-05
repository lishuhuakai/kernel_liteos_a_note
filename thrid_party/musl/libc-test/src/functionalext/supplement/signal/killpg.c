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

void ChildFunc(int signal)
{
    switch (signal) {
        case SIGCHLD:
            exit_flag = 1;
            break;
        default:
            break;
    }
    return;
}

/**
 * @tc.name      : killpg_0100
 * @tc.desc      : Determine whether the child process successfully receives the signal to
 *                 the main process and respond accordingly
 * @tc.level     : Level 1
 */
void killpg_0100(void)
{
    pid_t pid;
    int status;
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGCHLD);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    pid = fork();
    if (pid == 0) {
        sleep(1);
        int ret = killpg(getpgrp(), SIGCHLD);
        EXPECT_EQ("killpg_0100", ret, 0);
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        signal(SIGCHLD, ChildFunc);
        sigprocmask(SIG_UNBLOCK, &sigset, NULL);
        while (!exit_flag) {
            sleep(1);
        }
        wait(NULL);
        EXPECT_EQ("killpg_0100", exit_flag, 1);
    } else {
        printf("Fork wrong\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @tc.name      : killpg_0200
 * @tc.desc      : Outlier judgment
 * @tc.level     : Level 2
 */
void killpg_0200(void)
{
    int ret = killpg(-1, -1);
    EXPECT_EQ("killpg_0200", ret, -1);
}

int main(void)
{
    killpg_0100();
    killpg_0200();
    return t_status;
}
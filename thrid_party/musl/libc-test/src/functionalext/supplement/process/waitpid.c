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
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : waitpid_0100
 * @tc.desc      : Test the waitpid function to waitpid for the child process
 * @tc.level     : Level 0
 */
void waitpid_0100(void)
{
    pid_t pid = fork();
    if (pid > 0) {
        int status = 0;
        int options = 0;
        pid_t waitpid_for_pind = waitpid(pid, &status, options);
        if (waitpid_for_pind != pid) {
            t_error("%s waitpid get pid is %d are not want %d\n", __func__, waitpid_for_pind, pid);
        }
        if (status != 0) {
            t_error("%s waitpid get status is %d are not 0\n", __func__, status);
        }
    } else if (pid == 0) {
        sleep(1);
        exit(0);
    } else {
        t_error("%s waitpid fork error\n");
    }
}

/**
 * @tc.name      : waitpid_0200
 * @tc.desc      : Test the result of the waitpid function when the exit code of the child process is not 0
 * @tc.level     : Level 1
 */
void waitpid_0200(void)
{
    pid_t pid = fork();
    if (pid > 0) {
        int status = 0;
        int options = 0;
        pid_t waitpid_for_pind = waitpid(pid, &status, options);
        if (waitpid_for_pind != pid) {
            t_error("%s waitpid get pid is %d are not want %d\n", __func__, waitpid_for_pind, pid);
        }
        if (status == 0) {
            t_error("%s waitpid get status is 0\n", __func__);
        }
    } else if (pid == 0) {
        sleep(1);
        exit(66);
    } else {
        t_error("%s waitpid fork error\n");
    }
}

int main(int argc, char *argv[])
{
    waitpid_0100();
    sleep(2);
    waitpid_0200();
    return t_status;
}
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

const int EXIT_CODE = 66;
const int SLEEP_TIME = 2;

/**
 * @tc.name      : wait3_0100
 * @tc.desc      : Test the wait3 function to wait3 for the child process
 * @tc.level     : Level 0
 */
void wait3_0100(void)
{
    pid_t pid = fork();
    if (pid > 0) {
        int status = 0;
        int options = 0;
        struct rusage ru;
        pid_t wait3_for_pind = wait3(&status, options, &ru);
        if (wait3_for_pind != pid) {
            t_error("%s wait3 get pid is %d are not want %d\n", __func__, wait3_for_pind, pid);
        }
        if (status != 0) {
            t_error("%s wait3 get status is %d are not 0\n", __func__, status);
        }
    } else if (pid == 0) {
        sleep(1);
        exit(0);
    } else {
        t_error("%s wait3 fork error\n");
    }
}

/**
 * @tc.name      : wait3_0200
 * @tc.desc      : Test the result of the wait3 function when the exit code of the child process is not 0
 * @tc.level     : Level 1
 */
void wait3_0200(void)
{
    pid_t pid = fork();
    if (pid > 0) {
        int status = 0;
        int options = 0;
        struct rusage ru;
        pid_t wait3_for_pind = wait3(&status, options, &ru);
        if (wait3_for_pind != pid) {
            t_error("%s wait3 get pid is %d are not want %d\n", __func__, wait3_for_pind, pid);
        }
        if (status == 0) {
            t_error("%s wait3 get status is 0\n", __func__);
        }
    } else if (pid == 0) {
        sleep(1);
        exit(EXIT_CODE);
    } else {
        t_error("%s wait3 fork error\n");
    }
}

int main(int argc, char *argv[])
{
    wait3_0100();
    sleep(SLEEP_TIME);
    wait3_0200();
    return t_status;
}
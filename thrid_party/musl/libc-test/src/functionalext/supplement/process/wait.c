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
#include <unistd.h>
#include <sys/wait.h>
#include "test.h"

const int WAIT_TIME = 2;

/**
 * @tc.name      : wait_0100
 * @tc.desc      : Test the wait function to wait for the child process
 * @tc.level     : Level 0
 */
void wait_0100(void)
{
    pid_t pid = fork();
    if (pid > 0) {
        int flag = 0;
        pid_t wait_for_pind = wait(&flag);
        if (wait_for_pind != pid) {
            t_error("%s wait get pid is %d are not want %d\n", __func__, wait_for_pind, pid);
        }
        if (flag != 0) {
            t_error("%s wait get status is not 0\n", __func__);
        }
    } else if (pid == 0) {
        sleep(1);
        exit(0);
    } else {
        t_error("%s wait fork error\n");
    }
}

/**
 * @tc.name      : wait_0200
 * @tc.desc      : Test the result of the wait function when the exit code of the child process is not 0
 * @tc.level     : Level 1
 */
void wait_0200(void)
{
    pid_t pid = fork();
    if (pid > 0) {
        int flag = 0;
        pid_t wait_for_pind = wait(&flag);
        if (wait_for_pind != pid) {
            t_error("%s wait get pid is %d are not want %d\n", __func__, wait_for_pind, pid);
        }
        if (flag == 0) {
            t_error("%s wait get status is not 0\n", __func__);
        }
    } else if (pid == 0) {
        sleep(1);
        exit(66);
    } else {
        t_error("%s wait fork error\n");
    }
}

int main(int argc, char *argv[])
{
    wait_0100();
    sleep(WAIT_TIME);
    wait_0200();
    return t_status;
}
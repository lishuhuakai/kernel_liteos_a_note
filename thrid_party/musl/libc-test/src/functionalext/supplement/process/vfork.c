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
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : vfork_0100
 * @tc.desc      : The parent process will execute after the child process exits, and the two share memory space
 * @tc.level     : Level 0
 */
void vfork_0100()
{
    int a = 1;
    int b = 2;

    pid_t pid;
    pid = vfork();

    if (pid < 0) {
        t_error("%s vfork failed\n", __func__);
    } else if (pid == 0) {
        sleep(1);
        printf("child\n");
        a = 10;
        b = 20;
        _exit(0);
    } else if (pid > 0) {
        if (a == 1 || b == 2) {
            t_error("%s failed, a is %d, b is %d\n", __func__, a, b);
        }
    }
}

int main(int argc, char *argv[])
{
    vfork_0100();
    return t_status;
}
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

#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include "test.h"

/**
 * @tc.name      : system_0100
 * @tc.desc      : execute a shell command
 * @tc.level     : Level 0
 */
void system_0100(void)
{
    sighandler_t handler = signal(SIGCHLD, SIG_DFL);

    int result = system("cd /");
    if (result != 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }

    signal(SIGCHLD, handler);
}

/**
 * @tc.name      : system_0200
 * @tc.desc      : execute a shell command with NULL
 * @tc.level     : Level 2
 */
void system_0200(void)
{
    int result = system(NULL);
    if (result == 0) {
        t_error("%s failed: result = %d\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    system_0100();
    system_0200();

    return t_status;
}

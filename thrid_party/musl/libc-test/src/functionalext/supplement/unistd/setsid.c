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
#include <unistd.h>

#include "test.h"

/**
 * @tc.name      : setsid_0100
 * @tc.desc      : creates a session and sets the process group ID
 * @tc.level     : Level 0
 */
void setsid_0100(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        t_error("%s failed: pid = %d\n", __func__, pid);
    } else if (pid == 0) {
        pid_t result = setsid();
        if (result < 0) {
            t_error("%s failed: result = %d\n", __func__, result);
        }

        pid = getpid();
        if (pid < 0) {
            t_error("%s failed: pid = %d\n", __func__, pid);
        }

        pid_t sid = getsid(pid);
        if (sid < 0) {
            t_error("%s failed: sid = %d\n", __func__, sid);
        }

        if (result != sid) {
            t_error("%s failed: result = %d, sid = %d\n", __func__, result, sid);
        }
    } else {
        return;
    }
}

int main(int argc, char *argv[])
{
    setsid_0100();
    return t_status;
}

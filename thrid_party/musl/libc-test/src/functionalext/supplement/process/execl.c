/*
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

#include <errno.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : execl_0100
 * @tc.desc      : The file in the specified directory can be executed
 * @tc.level     : Level 0
 */
void execl_0100(void)
{
    errno = 0;
    pid_t fpid = fork();
    if (fpid == 0) {
        int result = execl("/bin/ls", "ls", "-al", "/etc/passwd", (char *)0);
        if (result == -1) {
            t_error("%s execl failed\n", __func__);
        }
        if (errno != 0) {
            t_error("%s failed, errno is %d\n", __func__, errno);
        }
    }
}

int main(int argc, char *argv[])
{
    execl_0100();
    return t_status;
}
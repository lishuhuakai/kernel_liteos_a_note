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

#include <sys/wait.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getpgrp_0100
 * @tc.desc      : Whether the child process is in the same process group as the parent process
 * @tc.level     : Level 0
 */
void getpgrp_0100(void)
{
    pid_t gpid = getpgrp();
    pid_t child = fork();
    EXPECT_TRUE("getpgrp_0100", child >= 0);

    if (child == 0) {
        pid_t gpid_child = getpgrp();
        EXPECT_EQ("getpgrp_0100", gpid_child, gpid);
    } else if (child > 0) {
        wait(NULL);
    }
}

int main(void)
{
    getpgrp_0100();
    return t_status;
}
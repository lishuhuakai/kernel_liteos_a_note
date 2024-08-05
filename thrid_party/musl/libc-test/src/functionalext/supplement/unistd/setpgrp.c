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
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : setpgrp_0100
 * @tc.desc      : Set the group ID of the current process as the process ID of the current process
 * @tc.level     : Level 0
 */
void setpgrp_0100(void)
{
    pid_t pt = setpgrp();
    if (pt == -1 && getpgrp() != getpid()) {
        t_error("%s setpgrp failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    setpgrp_0100();
    return t_status;
}
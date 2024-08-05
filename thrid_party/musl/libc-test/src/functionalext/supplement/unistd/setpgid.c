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
 * @tc.name      : setpgid_0100
 * @tc.desc      : Sets the process group ID of the process specified by pid to pgid
 * @tc.level     : Level 0
 */
void setpgid_0100(void)
{
    pid_t pid = 0;
    pid_t pgid = 0;
    int result = setpgid(pid, pgid);
    if (result < 0) {
        t_error("%s setpgid failed", __func__);
    }
    if (getgid() != result) {
        t_error("%s setpgid invalid", __func__);
    }
}

int main(int argc, char *argv[])
{
    setpgid_0100();
    return t_status;
}
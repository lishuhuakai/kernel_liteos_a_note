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
#include <sys/syscall.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : syscall_0100
 * @tc.desc      : Test used syscall to get thread kernel id
 * @tc.level     : Level 0
 */
void syscall_0100(void)
{
    pid_t tid;
    tid = syscall(SYS_gettid);
    if (tid == 0) {
        t_error("%s syscall SYS_gettid is 0\n", __func__);
    }
}

/**
 * @tc.name      : syscall_0200
 * @tc.desc      : Test used syscall to get gid
 * @tc.level     : Level 0
 */
void syscall_0200(void)
{
    gid_t gid;
    gid = syscall(SYS_getgid);
    if (gid != 0) {
        t_error("%s syscall SYS_getgid is not 0\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    syscall_0100();
    syscall_0200();
    return t_status;
}
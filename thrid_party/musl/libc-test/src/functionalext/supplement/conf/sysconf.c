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
#include <string.h>
#include "test.h"

/**
 * @tc.name      : sysconf_0100
 * @tc.desc      : The maximum number of processes running concurrently by the test user
 * @tc.level     : Level 0
 */
void sysconf_0100(void)
{
    long result = sysconf(_SC_CHILD_MAX);
    if (result == 0) {
        t_error("%s sysconf want get result is not zero", __func__);
    }
}

/**
 * @tc.name      : sysconf_0200
 * @tc.desc      : Get the maximum number of open files for a process
 * @tc.level     : Level 0
 */
void sysconf_0200(void)
{
    long result = sysconf(_SC_OPEN_MAX);
    if (result == 0) {
        t_error("%s sysconf want get result is not zero", __func__);
    }
}
int main(int argc, char *argv[])
{
    sysconf_0100();
    sysconf_0200();
    return t_status;
}
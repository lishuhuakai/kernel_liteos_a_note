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

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : strsignal_0100
 * @tc.desc      : Returns a string describing the signal number
 * @tc.level     : Level 0
 */
void strsignal_0100(void)
{
    char *str = strsignal(SIGHUP);
    if (strcmp(str, "Hangup")) {
        t_error("%s failed, str is %s", __func__, str);
    }
}

/**
 * @tc.name      : strsignal_0200
 * @tc.desc      : The parameter signum is equal to _NSIG
 * @tc.level     : Level 1
 */
void strsignal_0200(void)
{
    char *str = strsignal(_NSIG);
    if (strcmp(str, "Unknown signal")) {
        t_error("%s failed, str is %s\n", __func__, str);
    }
}

int main(int argc, char *argv[])
{
    strsignal_0100();
    strsignal_0200();
    return t_status;
}
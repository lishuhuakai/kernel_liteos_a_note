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


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "test.h"

/**
 * @tc.name      : unlockpt_0100
 * @tc.desc      : Test the unlockpt method to gain access to the pseudo-terminal
 * @tc.level     : Level 0
 */
void unlockpt_0100(void)
{
    int result;

    int fdm = open("/dev/ptmx", O_RDWR | O_NOCTTY);
    if (fdm < 0) {
        t_error("%s open master failed", __func__);
        return;
    }
    result = grantpt(fdm);
    if (result < 0) {
        t_error("%s grantpt failed", __func__);
        return;
    }

    result = unlockpt(fdm);
    if (result < 0) {
        t_error("%s unlockpt failed", __func__);
        return;
    }
}

int main(int argc, char *argv[])
{
    unlockpt_0100();
    return t_status;
}
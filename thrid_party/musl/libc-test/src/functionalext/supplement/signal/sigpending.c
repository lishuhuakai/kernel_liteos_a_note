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

#include "test.h"

/**
 * @tc.name      : sigpending_0100
 * @tc.desc      : Test the sigpending function to get the signal set set to SIG_BLOCK
 * @tc.level     : Level 0
 */
void sigpending_0100(void)
{
    sigset_t pending;
    sigemptyset(&pending);
    int result = sigpending(&pending);
    if (result != 0) {
        t_error("%s sigpending error get result is %d are not want 0\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    sigpending_0100();
    return t_status;
}
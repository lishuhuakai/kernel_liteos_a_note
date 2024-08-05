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
#include <stdlib.h>
#include <signal.h>
#include "test.h"

/**
 * @tc.name      : signal_0100
 * @tc.desc      : The test catches the exception through the signal function
 * @tc.level     : Level 0
 */
void signal_0100(int code)
{
    if (code != SIGILL & code != SIGTRAP) {
        t_error("%s signal get error code is %d are not SIGILL or SIGTRAP\n", __func__, code);
    }
    exit(t_status);
}

int main(int argc, char *argv[])
{
    int *p = NULL;
    signal(SIGILL, signal_0100);
    signal(SIGTRAP, signal_0100);
    *p = 0;
    return t_status;
}
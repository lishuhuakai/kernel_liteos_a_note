/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
#include "test.h"

static int exit_num = 0;
static int is_register = 0;

/**
 * @tc.name      : atexit_0100
 * @tc.desc      : Construct repeated call
 * @tc.level     : Level 0
 */
void atexit_0100(void)
{
    printf("%s is registered\n", __func__);
}

/**
 * @tc.name      : atexit_0200
 * @tc.desc      : Register atexit_0100 when execute atexit_0200
 * @tc.level     : Level 0
 */
void atexit_0200(void)
{
    exit_num++;
    if (!is_register) {
        int result = atexit(atexit_0100);
        if (result != 0) {
            t_error("atexit_0200 error get result is not 0\n");
        }
        is_register = 1;
    }
}

/**
 * @tc.name      : atexit_0300
 * @tc.desc      : Execute atexit_0300 when exit to check whether atexit_0200 is double-call
 * @tc.level     : Level 0
 */
void atexit_0300(void)
{
    if (exit_num > 1) {
        t_error("exit_num from atexit_0200 error get result is not 1, now is %d\n", exit_num);
    }
}

int main(int argc, char *argv[])
{
    int result = 0;
    result = atexit(atexit_0300);
    if (result != 0) {
        t_error("atexit_0300 error get result is not 0\n");
    }
    result = atexit(atexit_0200);
    if (result != 0) {
        t_error("atexit_0200 error get result is not 0\n");
    }
    return t_status;
}
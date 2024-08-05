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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : vprintf_0100
 * @tc.desc      : Send formatted output to standard output with %d
 * @tc.level     : Level 0
 */
void vprintf_0100(char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    if (result < 0) {
        t_error("%s vprintf get result is less than 0", __func__);
    }
    char *want = "This is the 1th test for vprintf";
    int len = strlen(want);
    if (result != len) {
        t_error("%s vprintf get wrong result, want %s", __func__, want);
    }
}

/**
 * @tc.name      : vprintf_0200
 * @tc.desc      : Send formatted output to standard output whit %s
 * @tc.level     : Level 1
 */
void vprintf_0200(char *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vprintf(format, args);
    va_end(args);
    if (result < 0) {
        t_error("%s vprintf get result is less than 0", __func__);
    }
    char *want = "This is the 2th test for vprintf";
    int len = strlen(want);
    if (result != len) {
        t_error("%s vprintf get wrong result, want %s", __func__, want);
    }
}

int main(int argc, char *argv[])
{
    vprintf_0100("This is the %dth test for vprintf", 1);
    vprintf_0200("This is the 2th test for %s", "vprintf");
    return t_status;
}
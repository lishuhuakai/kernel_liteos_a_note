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
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <wchar.h>
#include "test.h"

int readFile(wchar_t *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int result = vwprintf(fmt, ap);
    va_end(ap);
    return result;
}

/**
 * @tc.name      : vwprintf_0100
 * @tc.desc      : Test the vwprintf method to write a wide string to the console
 * @tc.level     : Level 0
 */
void vwprintf_0100(void)
{
    wchar_t *value = L"hello %ls\n";
    int resule = readFile(value, L"world");
    if (resule < 0) {
        t_error("%s vwprintf error get result is %d are not less 0\n", __func__, resule);
    }
}

int main(int argc, char *argv[])
{
    vwprintf_0100();
    return t_status;
}
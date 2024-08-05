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

void deal_aberrant(int code)
{
    if (code != SIGSEGV) {
        t_error("vfwprintf_0200 code is %d are not SIGSEGV", __func__, code);
    }
    exit(t_status);
}

int readFile(FILE *stream, wchar_t *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int result = vfwprintf(stream, fmt, ap);
    va_end(ap);
    return result;
}

/**
 * @tc.name      : vfwprintf_0100
 * @tc.desc      : Test vfwprintf method to write wide string to file
 * @tc.level     : Level 0
 */
void vfwprintf_0100(void)
{
    wchar_t *value = L"hello %ls";
    char *file_name = "/data/temp_vfwprintf.txt";
    FILE *file = fopen(file_name, "w");
    int resule = readFile(file, value, L"world");
    if (resule < 0) {
        t_error("%s vfwprintf error get result is %d are not less 0\n", __func__, resule);
    }
    fclose(file);
    unlink(file_name);
}

/**
 * @tc.name      : vfwprintf_0200
 * @tc.desc      : Test the result of the vfwprintf function when the incoming file is empty
 * @tc.level     : Level 2
 */
void vfwprintf_0200(void)
{
    wchar_t *value = L"hello %ls";
    signal(SIGSEGV, deal_aberrant);
    int resule = readFile(NULL, value, L"world");
}

int main(int argc, char *argv[])
{
    vfwprintf_0100();
    vfwprintf_0200();
    return t_status;
}
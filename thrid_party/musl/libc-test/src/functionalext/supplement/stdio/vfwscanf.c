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
#include <locale.h>
#include <wchar.h>
#include "test.h"

void deal_aberrant(int code)
{
    if (code != SIGSEGV) {
        t_error("vfwscanf_0200 code is %d are not SIGSEGV", __func__, code);
    }
    exit(t_status);
}

int read_from_file(FILE *fp, const wchar_t *format, ...)
{
    va_list args;
    va_start(args, format);
    int result = vfwscanf(fp, format, args);
    va_end(args);
    return result;
}

/**
 * @tc.name      : vfwscanf_0100
 * @tc.desc      : Test vfwscanf method to read wide string from file
 * @tc.level     : Level 0
 */
void vfwscanf_0100(void)
{
    setlocale(LC_ALL, "en_US.UTF-8");

    wchar_t symbol[] = L"\u0915\u0916\u0917\u0918\u0919";
    wchar_t names[5][5] = {L"Ka", L"Kha", L"Ga", L"Gha", L"Nga"};
    FILE *fp = fopen("example.txt", "w+");

    for (int i = 0; i < 5; i++)
        fwprintf(fp, L"%lc %ls ", symbol[i], names[i]);
    rewind(fp);

    wchar_t ch, str[5];
    for (int i = 0; i < 5; i++) {
        read_from_file(fp, L"%lc %ls ", &ch, str);
        if (wcscmp(str, names[i]) != 0) {
            t_error("%s vfwscanf in %d get result is %ls are not want %ls\n", __func__, i, str, names[i]);
        }
    }

    fclose(fp);
    unlink("example.txt");
}

/**
 * @tc.name      : vfwscanf_0200
 * @tc.desc      : Test the result of the vfwscanf function when the incoming file is empty
 * @tc.level     : Level 2
 */
void vfwscanf_0200(void)
{
    wchar_t symbol[] = L"\u0915\u0916\u0917\u0918\u0919";
    wchar_t names[5][5] = {L"Ka", L"Kha", L"Ga", L"Gha", L"Nga"};
    signal(SIGSEGV, deal_aberrant);
    read_from_file(NULL, symbol, names);
}

int main(int argc, char *argv[])
{
    vfwscanf_0100();
    vfwscanf_0200();
    return t_status;
}
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
#include <stdlib.h>
#include <signal.h>
#include <wchar.h>
#include <inttypes.h>
#include "test.h"

void deal_aberrant(int code)
{
    if (code != SIGSEGV) {
        t_error("wcstoumax_0200 code is %d are not SIGSEGV", __func__, code);
    }
    exit(t_status);
}

/**
 * @tc.name      : wcstoumax_0100
 * @tc.desc      : Test wcstoumax to get numeric part in wide string
 * @tc.level     : Level 0
 */
void wcstoumax_0100(void)
{
    wchar_t *end = NULL;
    uintmax_t want = 123U;
    uintmax_t result = wcstoumax(L"  +123x", &end, 10);
    if (result != want) {
        t_error("%s wcstoumax get result is %d are not want 123U\n", __func__, result);
    }
    if (wcscmp(end, L"x") != 0) {
        t_error("%s wcstoumax get end is %ls are not want x\n", __func__, end);
    }
}

/**
 * @tc.name      : wcstoumax_0200
 * @tc.desc      : Test the result of the wcstoumax method when the incoming parameter is empty
 * @tc.level     : Level 2
 */
void wcstoumax_0200(void)
{
    signal(SIGSEGV, deal_aberrant);
    uintmax_t result = wcstoumax(NULL, NULL, 0);
}

int main(int argc, char *argv[])
{
    wcstoumax_0100();
    wcstoumax_0200();
    return t_status;
}
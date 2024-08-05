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
#include <string.h>
#include <signal.h>
#include <wchar.h>
#include <inttypes.h>
#include "test.h"

void deal_aberrant(int code)
{
    if (code != SIGSEGV) {
        t_error("wcstoimax_0200 code is %d are not SIGSEGV", __func__, code);
    }
    exit(t_status);
}

/**
 * @tc.name      : wcstoimax_0100
 * @tc.desc      : Test wcstoimax to get numeric part in wide string
 * @tc.level     : Level 0
 */
void wcstoimax_0100(void)
{
    wchar_t *end = NULL;
    intmax_t want = 123;
    intmax_t result = wcstoimax(L"  +123x", &end, 10);
    if (result != want) {
        t_error("%s wcstoimax get result is %d are not want 123\n", __func__, result);
    }
    if (wcscmp(end, L"x") != 0) {
        t_error("%s wcstoimax get end is %ls are not want x\n", __func__, end);
    }
}

/**
 * @tc.name      : wcstoimax_0200
 * @tc.desc      : Test the result of the wcstoimax method when the incoming parameter is empty
 * @tc.level     : Level 2
 */
void wcstoimax_0200(void)
{
    signal(SIGSEGV, deal_aberrant);
    intmax_t result = wcstoimax(NULL, NULL, 0);
}

int main(int argc, char *argv[])
{
    wcstoimax_0100();
    wcstoimax_0200();
    return t_status;
}
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
#include "test.h"

void deal_aberrant(int code)
{
    if (code != SIGSEGV) {
        t_error("wcsdup_0200 code is %d are not SIGSEGV", __func__, code);
    }
    exit(t_status);
}

/**
 * @tc.name      : wcsdup_0100
 * @tc.desc      : Call wcsdup to get a newly allocated wide string
 * @tc.level     : Level 0
 */
void wcsdup_0100(void)
{
    wchar_t *src = L"hello";
    wchar_t *result = wcsdup(src);
    if (wcscmp(src, result) != 0) {
        t_error("%s wcscmp get result is %ls are not want %ls\n", __func__, result, src);
    }
}

/**
 * @tc.name      : wcsdup_0200
 * @tc.desc      : Test the wcsdup result when the incoming null
 * @tc.level     : Level 2
 */
void wcsdup_0200(void)
{
    signal(SIGSEGV, deal_aberrant);
    wchar_t *result = wcsdup(NULL);
}

int main(int argc, char *argv[])
{
    wcsdup_0100();
    wcsdup_0200();
    return t_status;
}
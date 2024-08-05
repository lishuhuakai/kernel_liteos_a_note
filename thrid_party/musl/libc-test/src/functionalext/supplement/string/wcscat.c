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

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "test.h"

#define MAX (20)

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : wcscat_0100
 * @tc.desc      : Concatenate wide strings
 * @tc.level     : Level 0
 */
void wcscat_0100(void)
{
    wchar_t dest[MAX] = L"wcscat ";
    wchar_t src[MAX] = L"test";
    wcscat(dest, src);
    if (wcscmp(dest, L"wcscat test")) {
        t_error("%s concatenate wide strings failed\n", __func__);
    }
}

/**
 * @tc.name      : wcscat_0200
 * @tc.desc      : destination and source overlap.
 * @tc.level     : Level 2
 */
void wcscat_0200(void)
{
    signal(SIGSEGV, handler);

    wchar_t dest[MAX] = L"wcscat ";
    wchar_t *src = &dest[1];
    wcscat(dest, src);
}

int main(int argc, char *argv[])
{
    wcscat_0100();
    wcscat_0200();
    return t_status;
}
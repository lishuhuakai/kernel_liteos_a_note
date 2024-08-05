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

#define BUFF_SIZE (40)

void handler(int sig)
{
    exit(t_status);
}

/**
 * @tc.name      : wcscpy_0100
 * @tc.desc      : Copies the C wide string pointed by wcs into the array pointed by des
 * @tc.level     : Level 0
 */
void wcscpy_0100(void)
{
    wchar_t wcs[] = L"wcscpy test";
    wchar_t des[BUFF_SIZE];
    wcscpy(des, wcs);
    if (wcscmp(wcs, des)) {
        t_error("%s wcscpy failed", __func__);
    }
}

/**
 * @tc.name      : wcscpy_0200
 * @tc.desc      : Copy when wcs is an empty string
 * @tc.level     : Level 1
 */
void wcscpy_0200(void)
{
    wchar_t wcs[] = L"";
    wchar_t des[] = L"wcscpy test";
    wcscpy(des, wcs);
    if (wcscmp(wcs, des)) {
        t_error("%s des is not equal to wcs", __func__);
    }
}

/**
 * @tc.name      : wcscpy_0300
 * @tc.desc      : des is empty, the program is abnormal
 * @tc.level     : Level 2
 */
void wcscpy_0300(void)
{
    signal(SIGSEGV, handler);

    wchar_t wcs[] = L"wcscpy test";
    wcscpy(NULL, wcs);
}

int main(int argc, char *argv[])
{
    wcscpy_0100();
    wcscpy_0200();
    wcscpy_0300();
    return t_status;
}
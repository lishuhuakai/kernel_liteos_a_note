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
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wmemmove_0100
 * @tc.desc      : Copies the specified number of wide characters from the source to the destination
 * @tc.level     : Level 0
 */
void wmemmove_0100(void)
{
    wchar_t dest[] = L"This is a c test for wmemmove function";
    const wchar_t src[] = L"src content";
    int count = 5;
    wchar_t tmp[count + 1];
    wcsncpy(tmp, src, count);
    wmemmove(dest, src, count);
    if (wcsncmp(dest, tmp, count)) {
        t_error("The dest specified bits are not equal to the src\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0200
 * @tc.desc      : Dest and src overlaps
 * @tc.level     : Level 1
 */
void wmemmove_0200(void)
{
    wchar_t src[] = L"This is a c test for wmemmove function";
    wchar_t *dest = &src[2];
    int count = 5;
    wchar_t tmp[count + 1];
    wcsncpy(tmp, src, count);
    wmemmove(dest, src, count);
    if (wcsncmp(dest, tmp, count)) {
        t_error("The dest specified bits are not equal to the src\n", __func__);
    }
}

/**
 * @tc.name      : wmemmove_0300
 * @tc.desc      : The first address of dest is the same as src
 * @tc.level     : Level 1
 */
void wmemmove_0300(void)
{
    wchar_t src[] = L"This is a c test for wmemmove function";
    wchar_t *dest = &src[0];
    int count = 5;
    wchar_t *result = wmemmove(dest, src, count);
    if (!(result == dest)) {
        t_error("The first address of dest is not the same as src\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wmemmove_0100();
    wmemmove_0200();
    wmemmove_0300();
    return t_status;
}
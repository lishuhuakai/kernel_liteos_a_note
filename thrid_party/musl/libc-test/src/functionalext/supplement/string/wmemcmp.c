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
 * @tc.name      : wmemcmp_0100
 * @tc.desc      : Test the return value of wmemcmp when two strings are equal
 * @tc.level     : Level 0
 */
void wmemcmp_0100(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCD";
    int result = wmemcmp(l, r, sizeof(l) / sizeof(*l));
    if (result != 0) {
        t_error("%s two strings are not equal\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0200
 * @tc.desc      : l precedes r in lexicographical order
 * @tc.level     : Level 1
 */
void wmemcmp_0200(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCE";
    int result = wmemcmp(l, r, sizeof(l) / sizeof(*l));
    if (result >= 0) {
        t_error("%s l is not precedes to r\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0300
 * @tc.desc      : r precedes l in lexicographical order
 * @tc.level     : Level 1
 */
void wmemcmp_0300(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCC";
    int result = wmemcmp(l, r, sizeof(l) / sizeof(*l));
    if (result <= 0) {
        t_error("%s l is not follows to r\n", __func__);
    }
}

/**
 * @tc.name      : wmemcmp_0400
 * @tc.desc      : Test the return value of the function when n=0
 * @tc.level     : Level 1
 */
void wmemcmp_0400(void)
{
    wchar_t l[] = L"ABCD";
    wchar_t r[] = L"ABCC";
    int n = 0;
    int result = wmemcmp(l, r, n);
    if (result != 0) {
        t_error("%s result is not want 0\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wmemcmp_0100();
    wmemcmp_0200();
    wmemcmp_0300();
    wmemcmp_0400();
    return t_status;
}
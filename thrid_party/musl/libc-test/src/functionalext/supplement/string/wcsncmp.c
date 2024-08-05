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
 * @tc.name      : wcsncmp_0100
 * @tc.desc      : Test the wcsncmp function to compare the first n bits of two wide strings
 * @tc.level     : Level 0
 */
void wcsncmp_0100(void)
{
    int result = wcsncmp(L"aaaa", L"aaab", 3);
    if (result != 0) {
        t_error("%s wcsncmp get result is %d are not 0", __func__, result);
    }
}

/**
 * @tc.name      : wcsncmp_0200
 * @tc.desc      : Test the wcsncmp result when comparing the top 0 bits
 * @tc.level     : Level 1
 */
void wcsncmp_0200(void)
{
    int result = wcsncmp(L"foo", L"bar", 0);
    if (result != 0) {
        t_error("%s wcsncmp get result is %d are not 0", __func__, result);
    }
}

/**
 * @tc.name      : wcsncmp_0300
 * @tc.desc      : Test wcsncmp result when first wide string is lexicographically lower than second wide string
 * @tc.level     : Level 1
 */
void wcsncmp_0300(void)
{
    int result = wcsncmp(L"aaaa", L"aaab", 4);
    if (result >= 0) {
        t_error("%s wcsncmp get result is %d are not less 0", __func__, result);
    }
}

/**
 * @tc.name      : wcsncmp_0400
 * @tc.desc      : Test wcsncmp result when first wide string is lexicographically higher than second wide string
 * @tc.level     : Level 1
 */
void wcsncmp_0400(void)
{
    int result = wcsncmp(L"aaab", L"aaaa", 4);
    if (result <= 0) {
        t_error("%s wcsncmp get result is %d are not more 0", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsncmp_0100();
    wcsncmp_0200();
    wcsncmp_0300();
    wcsncmp_0400();
    return t_status;
}
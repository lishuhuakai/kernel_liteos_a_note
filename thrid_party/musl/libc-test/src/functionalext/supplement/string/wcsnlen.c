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
 * @tc.name      : wcsnlen_0100
 * @tc.desc      : Test the wcsnlen method to get the number of wide strings
 * @tc.level     : Level 0
 */
void wcsnlen_0100(void)
{
    wchar_t *wc = L"ABCDEF";
    size_t want = 6;
    size_t result = wcsnlen(wc, 6);
    if (result != want) {
        t_error("%s wcsnlen error get result is %d are not want 6\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsnlen_0200
 * @tc.desc      : Test the return value of wcsnlen when the incoming length is 0
 * @tc.level     : Level 1
 */
void wcsnlen_0200(void)
{
    wchar_t *wc = L"ABCDEF";
    size_t result = wcsnlen(wc, 0);
    if (result != 0) {
        t_error("%s wcsnlen error get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsnlen_0300
 * @tc.desc      : Test the return value of wcsnlen when the incoming length is less than the incoming wide string
 * @tc.level     : Level 1
 */
void wcsnlen_0300(void)
{
    wchar_t *wc = L"ABCDEF";
    size_t want = 3;
    size_t result = wcsnlen(wc, 3);
    if (result != want) {
        t_error("%s wcsnlen error get result is %d are not want 3\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsnlen_0400
 * @tc.desc      : Test the return value of wcsnlen when the incoming length is greater than the incoming wide string
 *                 length
 * @tc.level     : Level 1
 */
void wcsnlen_0400(void)
{
    wchar_t *wc = L"ABCDEF";
    size_t want = 6;
    size_t result = wcsnlen(wc, 7);
    if (result != want) {
        t_error("%s wcsnlen error get result is %d are not want 6\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsnlen_0100();
    wcsnlen_0200();
    wcsnlen_0300();
    wcsnlen_0400();
    return t_status;
}
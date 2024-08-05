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
 * @tc.name      : wcscspn_0100
 * @tc.desc      : Call the wcscspn method to get the length of the maximum initial segment of the wide string points
 *                 to dest
 * @tc.level     : Level 0
 */
void wcscspn_0100(void)
{
    size_t result = wcscspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz");
    if (result != 0U) {
        t_error("%s wcscspn get result is %d are not want 0U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0200
 * @tc.desc      : The return result of wcscspn when dest is a space
 * @tc.level     : Level 1
 */
void wcscspn_0200(void)
{
    size_t result = wcscspn(L"hello world", L" ");
    size_t want = 5U;
    if (result != want) {
        t_error("%s wcscspn get result is %d are not want 5U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscspn_0300
 * @tc.desc      : wcscspn returns result when dest is punctuation
 * @tc.level     : Level 1
 */
void wcscspn_0300(void)
{
    size_t result = wcscspn(L"hello world", L"!");
    size_t want = 11U;
    if (result != want) {
        t_error("%s wcscspn get result is %d are not want 11U\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcscspn_0100();
    wcscspn_0200();
    wcscspn_0300();
    return t_status;
}
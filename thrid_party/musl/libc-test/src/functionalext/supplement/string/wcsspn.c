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
 * @tc.name      : wcsspn_0100
 * @tc.desc      : Test the wcsspn function to get the length of the largest initial segment of the wide string pointed
 * @tc.level     : Level 0
 */
void wcsspn_0100(void)
{
    size_t result = wcsspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz");
    size_t want = 5U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 5U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0200
 * @tc.desc      : Test the wcsspn result when the search string contains spaces
 * @tc.level     : Level 1
 */
void wcsspn_0200(void)
{
    size_t result = wcsspn(L"hello world", L"abcdefghijklmnopqrstuvwxyz ");
    size_t want = 11U;
    if (result != want) {
        t_error("%s wcsspn get result is %d are not want 11U\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsspn_0300
 * @tc.desc      : The result of the wcsspn function when no target exists for the retrieved string
 * @tc.level     : Level 1
 */
void wcsspn_0300(void)
{
    size_t result = wcsspn(L"hello world", L"!");
    if (result != 0U) {
        t_error("%s wcsspn get result is %d are not want 0U\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsspn_0100();
    wcsspn_0200();
    wcsspn_0300();
    return t_status;
}
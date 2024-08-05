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
#include <ctype.h>
#include "test.h"

const int WIDTH = 2;
const int ERR_RESULT = -1;

/**
 * @tc.name      : wcswidth_0100
 * @tc.desc      : Test wcswidth to get the number of wide string columns
 * @tc.level     : Level 0
 */
void wcswidth_0100(void)
{
    int result = wcswidth(L"abc", WIDTH);
    if (result != WIDTH) {
        t_error("%s wcswidth get result is %s are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcswidth_0200
 * @tc.desc      : wcswidth result when wide string contains non-printing characters
 * @tc.level     : Level 1
 */
void wcswidth_0200(void)
{
    int result = wcswidth(L"ab\t", WIDTH);
    if (result != WIDTH) {
        t_error("%s wcswidth get result is %s are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcswidth_0300
 * @tc.desc      : The result of wcswidth when nonprinting characters are included in the selection
 * @tc.level     : Level 2
 */
void wcswidth_0300(void)
{
    int result = wcswidth(L"a\tb", WIDTH);
    if (result != ERR_RESULT) {
        t_error("%s wcswidth get result is %s are not want -1\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcswidth_0100();
    wcswidth_0200();
    wcswidth_0300();
    return t_status;
}
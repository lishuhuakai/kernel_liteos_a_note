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
#include <locale.h>
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wctomb_0100
 * @tc.desc      : Test the wctomb method to copy wide characters into a string
 * @tc.level     : Level 0
 */
void wctomb_0100(void)
{
    char byte[256] = {0};
    int result = wctomb(byte, L'h');
    if (result != 1) {
        t_error("%s wctomb get result is %d are not want 1\n", __func__, result);
    }
}

/**
 * @tc.name      : wctomb_0200
 * @tc.desc      : Test the result of wctomb when the incoming wide character is the terminator
 * @tc.level     : Level 1
 */
void wctomb_0200(void)
{
    char byte[256] = {0};
    int result = wctomb(byte, L'\0');
    if (result != 1) {
        t_error("%s wctomb get result is %d are not want 1\n", __func__, result);
    }
}

/**
 * @tc.name      : wctomb_0300
 * @tc.desc      : wctomb result when test string is NULL
 * @tc.level     : Level 2
 */
void wctomb_0300(void)
{
    int result = wctomb(NULL, L'h');
    if (result != 0) {
        t_error("%s wctomb get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wctomb_0400
 * @tc.desc      : wctomb result when test string is NULL and wide character is terminator
 * @tc.level     : Level 2
 */
void wctomb_0400(void)
{
    int result = wctomb(NULL, L'\0');
    if (result != 0) {
        t_error("%s wctomb get result is %d are not want 0\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wctomb_0100();
    wctomb_0200();
    wctomb_0300();
    wctomb_0400();
    return t_status;
}
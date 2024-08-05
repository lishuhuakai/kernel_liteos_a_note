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

#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : wcsncasecmp_l_0100
 * @tc.desc      : Test wcsncasecmp_l ignore case compares the first n digits of two wide strings
 * @tc.level     : Level 0
 */
void wcsncasecmp_l_0100(void)
{
    int result = wcsncasecmp_l(L"hello1", L"HELLO2", 5, NULL);
    if (result != 0) {
        t_error("%s wcsncasecmp_l get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_l_0200
 * @tc.desc      : Test that wcsncasecmp_l returns the result when ws1 is greater than ws2 in the dictionary
 * @tc.level     : Level 1
 */
void wcsncasecmp_l_0200(void)
{
    int result = wcsncasecmp_l(L"hello1", L"HELLO2", 6, NULL);
    if (result >= 0) {
        t_error("%s wcsncasecmp_l get result is %d are not want less 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_l_0300
 * @tc.desc      : Test that wcsncasecmp_l returns the result when ws1 is less than ws2 in the dictionary
 * @tc.level     : Level 1
 */
void wcsncasecmp_l_0300(void)
{
    int result = wcsncasecmp_l(L"hello2", L"HELLO1", 6, NULL);
    if (result <= 0) {
        t_error("%s wcsncasecmp_l get result is %d are not want gress 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_l_0400
 * @tc.desc      : Test that wcsncasecmp_l returns the result when ws1 is longer in the dictionary than ws2
 * @tc.level     : Level 1
 */
void wcsncasecmp_l_0400(void)
{
    int result = wcsncasecmp_l(L"hello", L"HELL", 5, NULL);
    if (result <= 0) {
        t_error("%s wcsncasecmp_l get result is %d are not want gress 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_l_0500
 * @tc.desc      : wcsncasecmp_l returns result when test ws1 is shorter than ws2 in dictionary
 * @tc.level     : Level 1
 */
void wcsncasecmp_l_0500(void)
{
    int result = wcsncasecmp_l(L"hell", L"HELLO", 5, NULL);
    if (result >= 0) {
        t_error("%s wcsncasecmp_l get result is %d are not want less 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_l_0600
 * @tc.desc      : Test the result returned by wcsncasecmp_l when the number of comparisons is 0
 * @tc.level     : Level 2
 */
void wcsncasecmp_l_0600(void)
{
    int result = wcsncasecmp_l(L"foo", L"bar", 0, NULL);
    if (result != 0) {
        t_error("%s wcsncasecmp_l get result is %d are not want 0\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsncasecmp_l_0100();
    wcsncasecmp_l_0200();
    wcsncasecmp_l_0300();
    wcsncasecmp_l_0400();
    wcsncasecmp_l_0500();
    wcsncasecmp_l_0600();
    return t_status;
}
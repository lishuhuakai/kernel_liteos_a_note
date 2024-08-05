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
 * @tc.name      : wcsncasecmp_0100
 * @tc.desc      : Test wcsncasecmp ignore case compares the first n digits of two wide strings
 * @tc.level     : Level 0
 */
void wcsncasecmp_0100(void)
{
    int result = wcsncasecmp(L"hello1", L"HELLO2", 5);
    if (result != 0) {
        t_error("%s wcsncasecmp get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_0200
 * @tc.desc      : Test that wcsncasecmp returns the result when ws1 is greater than ws2 in the dictionary
 * @tc.level     : Level 1
 */
void wcsncasecmp_0200(void)
{
    int result = wcsncasecmp(L"hello1", L"HELLO2", 6);
    if (result >= 0) {
        t_error("%s wcsncasecmp get result is %d are not want less 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_0300
 * @tc.desc      : Test that wcsncasecmp returns the result when ws1 is less than ws2 in the dictionary
 * @tc.level     : Level 1
 */
void wcsncasecmp_0300(void)
{
    int result = wcsncasecmp(L"hello2", L"HELLO1", 6);
    if (result <= 0) {
        t_error("%s wcsncasecmp get result is %d are not want gress 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_0400
 * @tc.desc      : Test that wcsncasecmp returns the result when ws1 is longer in the dictionary than ws2
 * @tc.level     : Level 1
 */
void wcsncasecmp_0400(void)
{
    int result = wcsncasecmp(L"hello", L"HELL", 5);
    if (result <= 0) {
        t_error("%s wcsncasecmp get result is %d are not want gress 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_0500
 * @tc.desc      : wcsncasecmp returns result when test ws1 is shorter than ws2 in dictionary
 * @tc.level     : Level 1
 */
void wcsncasecmp_0500(void)
{
    int result = wcsncasecmp(L"hell", L"HELLO", 5);
    if (result >= 0) {
        t_error("%s wcsncasecmp get result is %d are not want less 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcsncasecmp_0600
 * @tc.desc      : Test the result returned by wcsncasecmp when the number of comparisons is 0
 * @tc.level     : Level 2
 */
void wcsncasecmp_0600(void)
{
    int result = wcsncasecmp(L"foo", L"bar", 0);
    if (result != 0) {
        t_error("%s wcsncasecmp get result is %d are not want 0\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcsncasecmp_0100();
    wcsncasecmp_0200();
    wcsncasecmp_0300();
    wcsncasecmp_0400();
    wcsncasecmp_0500();
    wcsncasecmp_0600();
    return t_status;
}
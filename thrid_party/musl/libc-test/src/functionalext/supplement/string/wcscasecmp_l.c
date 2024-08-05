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
#include <locale.h>
#include <bits/alltypes.h>
#include "test.h"

/**
 * @tc.name      : wcscasecmp_l_0100
 * @tc.desc      : Test wcscasecmp_l compare case-ignoring wide strings
 * @tc.level     : Level 0
 */
void wcscasecmp_l_0100(void)
{
    locale_t local_info = NULL;
    int result = wcscasecmp_l(L"hello", L"HELLO", local_info);
    if (result != 0) {
        t_error("%s wcscasecmp_l get result is %d are not 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscasecmp_l_0200
 * @tc.desc      : Test the result of calling wcscasecmp_l with uppercase letters, plus numbers, and lowercase letters
 *                 and small numbers
 * @tc.level     : Level 1
 */
void wcscasecmp_l_0200(void)
{
    locale_t local_info = NULL;
    int result = wcscasecmp_l(L"hello1", L"HELLO2", local_info);
    if (result >= 0) {
        t_error("%s wcscasecmp_l get result is %d are more then 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscasecmp_l_0300
 * @tc.desc      : Test the result of calling wcscasecmp_l with uppercase letters plus small numbers and lowercase
 *                 letters and large numbers
 * @tc.level     : Level 1
 */
void wcscasecmp_l_0300(void)
{
    locale_t local_info = NULL;
    int result = wcscasecmp_l(L"hello2", L"HELLO1", local_info);
    if (result <= 0) {
        t_error("%s wcscasecmp_l get result is %d are less then 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscasecmp_l_0400
 * @tc.desc      : Test the result of calling wcscasecmp_l for shorter uppercase wide strings versus longer lowercase
 *                 wide strings
 * @tc.level     : Level 1
 */
void wcscasecmp_l_0400(void)
{
    locale_t local_info = NULL;
    int result = wcscasecmp_l(L"hello", L"HELL", local_info);
    if (result <= 0) {
        t_error("%s wcscasecmp_l get result is %d are less then 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscasecmp_l_0500
 * @tc.desc      : Test the result of calling wcscasecmp_l on a shorter lowercase wide string versus a longer uppercase
 *                 wide string
 * @tc.level     : Level 1
 */
void wcscasecmp_l_0500(void)
{
    locale_t local_info = NULL;
    int result = wcscasecmp_l(L"hell", L"HELLO", local_info);
    if (result >= 0) {
        t_error("%s wcscasecmp_l get result is %d are more then 0\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcscasecmp_l_0100();
    wcscasecmp_l_0200();
    wcscasecmp_l_0300();
    wcscasecmp_l_0400();
    wcscasecmp_l_0500();
    return t_status;
}
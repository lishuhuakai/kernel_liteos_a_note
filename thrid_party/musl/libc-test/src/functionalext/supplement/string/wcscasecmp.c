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
 * @tc.name      : wcscasecmp_0100
 * @tc.desc      : Test wcscasecmp compare case-ignoring wide strings
 * @tc.level     : Level 0
 */
void wcscasecmp_0100(void)
{
    int result = wcscasecmp(L"hello", L"HELLO");
    if (result != 0) {
        t_error("%s wcscasecmp get result is %d are not 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscasecmp_0200
 * @tc.desc      : Test the result of calling wcscasecmp with uppercase letters, plus numbers, and lowercase letters 
 *                 and small numbers
 * @tc.level     : Level 1
 */
void wcscasecmp_0200(void)
{
    int result = wcscasecmp(L"hello1", L"HELLO2");
    if (result >= 0) {
        t_error("%s wcscasecmp get result is %d are more then 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscasecmp_0300
 * @tc.desc      : Test the result of calling wcscasecmp with uppercase letters plus small numbers and lowercase 
 *                 letters and large numbers
 * @tc.level     : Level 1
 */
void wcscasecmp_0300(void)
{
    int result = wcscasecmp(L"hello2", L"HELLO1");
    if (result <= 0) {
        t_error("%s wcscasecmp get result is %d are less then 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscasecmp_0400
 * @tc.desc      : Test the result of calling wcscasecmp for shorter uppercase wide strings versus longer lowercase 
 *                 wide strings
 * @tc.level     : Level 1
 */
void wcscasecmp_0400(void)
{
    int result = wcscasecmp(L"hello", L"HELL");
    if (result <= 0) {
        t_error("%s wcscasecmp get result is %d are less then 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcscasecmp_0500
 * @tc.desc      : Test the result of calling wcscasecmp on a shorter lowercase wide string versus a longer uppercase
 *                 wide string
 * @tc.level     : Level 1
 */
void wcscasecmp_0500(void)
{
    int result = wcscasecmp(L"hell", L"HELLO");
    if (result >= 0) {
        t_error("%s wcscasecmp get result is %d are more then 0\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcscasecmp_0100();
    wcscasecmp_0200();
    wcscasecmp_0300();
    wcscasecmp_0400();
    wcscasecmp_0500();
    return t_status;
}
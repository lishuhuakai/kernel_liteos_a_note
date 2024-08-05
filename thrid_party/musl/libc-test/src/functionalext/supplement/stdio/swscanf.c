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
#include <wchar.h>
#include "test.h"

#define BUFF_SIZE (20)
#define RESULT_VAL (2)

/**
 * @tc.name      : swscanf_0100
 * @tc.desc      : Reads data from the wide string ws and stores them according to parameter format
 * @tc.level     : Level 0
 */
void swscanf_0100(void)
{
    wchar_t wstr[] = L"swscanf 123";
    wchar_t tmp[BUFF_SIZE];
    int i;
    int result = swscanf(wstr, L"%ls %d", tmp, &i);
    if (result == EOF) {
        t_error("%s swscanf failed\n", __func__);
        return;
    }

    if (i != 123) {
        t_error("%s i = %d is not want 123\n", __func__, i);
    }
    if (wcscmp(tmp, L"swscanf")) {
        t_error("%s wide string is %s not swscanf\n", __func__, tmp);
    }
}

/**
 * @tc.name      : swscanf_0200
 * @tc.desc      : Different formatted data
 * @tc.level     : Level 1
 */
void swscanf_0200(void)
{
    wchar_t wstr[] = L"swscanf 123";
    wchar_t tmp1[BUFF_SIZE];
    wchar_t tmp2[BUFF_SIZE];
    int i;
    int result = swscanf(wstr, L"%ls %ls", tmp1, tmp2);
    if (result == EOF) {
        t_error("%s swscanf failed\n", __func__);
        return;
    }
    if (wcscmp(tmp1, L"swscanf")) {
        t_error("%s tmp1 = %s is not want swscanf\n", __func__, tmp1);
    }
    if (wcscmp(tmp2, L"123")) {
        t_error("%s tmp2 = %s is not want 123\n", __func__, tmp2);
    }
}

/**
 * @tc.name      : swscanf_0300
 * @tc.desc      : Formatting exception
 * @tc.level     : Level 2
 */
void swscanf_0300(void)
{
    wchar_t wstr[] = L"swscanf 123";
    int i, j;
    int result = swscanf(wstr, L"%d %d", &i, &j);
    if (result >= RESULT_VAL) {
        t_error("%s swscanf expect failed\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    swscanf_0100();
    swscanf_0200();
    swscanf_0300();
    return t_status;
}
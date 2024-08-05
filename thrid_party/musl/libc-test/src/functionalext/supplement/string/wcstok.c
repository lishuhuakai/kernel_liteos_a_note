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
#include <locale.h>
#include <string.h>
#include "test.h"

/**
 * @tc.name      : wcstok_0100
 * @tc.desc      : Test wcstok to intercept the target string according to the incoming wide string
 * @tc.level     : Level 0
 */
void wcstok_0100(void)
{
    wchar_t str[] = L"parrot,owl,sparrow,pigeon,crow";
    wchar_t delim[] = L",";
    wchar_t *ptr;
    wchar_t *compare[5] = {L"parrot", L"owl", L"sparrow", L"pigeon", L"crow"};
    wchar_t *result[5] = {0};
    wchar_t *token = wcstok(str, delim, &ptr);
    int i = 0;
    while (token) {
        result[i] = token;
        i++;
        token = wcstok(NULL, delim, &ptr);
    }
    for (i = 0; i < 5; i++) {
        if (wcscmp(result[i], compare[i]) != 0) {
            t_error("%s wcstok in %d get value is %s and %s\n", __func__, i, result[i], compare[i]);
        }
    }
}

/**
 * @tc.name      : wcstok_0200
 * @tc.desc      : Test wcstok to intercept multiple targets according to incoming
 * @tc.level     : Level 1
 */
void wcstok_0200(void)
{
    wchar_t str[] = L"parrot,owl.sparrow,pigeon,crow";
    wchar_t delim[] = L",.";
    wchar_t *ptr;
    wchar_t *result = wcstok(str, delim, &ptr);
    if (wcscmp(result, L"parrot") != 0) {
        t_error("%s wcstok get result is %s are not parrot\n", __func__, result);
    }
    if (wcscmp(ptr, L"owl.sparrow,pigeon,crow") != 0) {
        t_error("%s wcstok get ptr is %s are not parrot\n", __func__, ptr);
    }
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_US.utf8");
    wcstok_0100();
    wcstok_0200();
    return t_status;
}
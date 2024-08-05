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

#define N(s, sub)                                                                  \
    {                                                                              \
        wchar_t *p = s;                                                            \
        wchar_t *q = wcswcs(p, sub);                                               \
        if (q)                                                                     \
            t_error("wcswcs(%s,%s) returned str+%d, wanted 0\n", #s, #sub, q - p); \
    }

#define T(s, sub, n)                                                                       \
    {                                                                                      \
        wchar_t *p = s;                                                                    \
        wchar_t *q = wcswcs(p, sub);                                                       \
        if (q == 0)                                                                        \
            t_error("wcswcs(%s,%s) returned 0, wanted str+%d\n", #s, #sub, n);             \
        else if (q - p != n)                                                               \
            t_error("wcswcs(%s,%s) returned str+%d, wanted str+%d\n", #s, #sub, q - p, n); \
    }

/**
 * @tc.name      : wcswcs_0100
 * @tc.desc      : Test the wcswcs method to find the first occurrence of the second wide string from the first wide
 *                 string
 * @tc.level     : Level 0
 */
void wcswcs_0100(void)
{
    T(L"", L"", 0)
    T(L"abcd", L"", 0)
    T(L"abcd", L"a", 0)
    T(L"abcd", L"b", 1)
    T(L"abcd", L"c", 2)
    T(L"abcd", L"d", 3)
    T(L"abcd", L"ab", 0)
    T(L"abcd", L"bc", 1)
    T(L"abcd", L"cd", 2)
    T(L"ababa", L"baba", 1)
    T(L"ababab", L"babab", 1)
    T(L"abababa", L"bababa", 1)
    T(L"abababab", L"bababab", 1)
    T(L"ababababa", L"babababa", 1)
    T(L"abbababab", L"bababa", 2)
    T(L"abbababab", L"ababab", 3)
    T(L"abacabcabcab", L"abcabcab", 4)
    T(L"nanabanabanana", L"aba", 3)
    T(L"nanabanabanana", L"ban", 4)
    T(L"nanabanabanana", L"anab", 1)
    T(L"nanabanabanana", L"banana", 8)
    T(L"_ _\xff_ _", L"_\xff_", 2)
}

/**
 * @tc.name      : wcswcs_0200
 * @tc.desc      : Test the result of the wcswcs method when the second substring cannot be found from the first
 * substring
 * @tc.level     : Level 1
 */
void wcswcs_0200(void)
{
    N(L"", L"a")
    N(L"a", L"aa")
    N(L"a", L"b")
    N(L"aa", L"ab")
    N(L"aa", L"aaa")
    N(L"abba", L"aba")
    N(L"abc abc", L"abcd")
    N(L"0-1-2-3-4-5-6-7-8-9", L"-3-4-56-7-8-")
    N(L"0-1-2-3-4-5-6-7-8-9", L"-3-4-5+6-7-8-")
    N(L"_ _ _\xff_ _ _", L"_\x7f_")
    N(L"_ _ _\x7f_ _ _", L"_\xff_")
}

int main(int argc, char *argv[])
{
    wcswcs_0100();
    wcswcs_0200();
    return t_status;
}
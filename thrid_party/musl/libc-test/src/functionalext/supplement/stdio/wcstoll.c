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

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "test.h"

#define TEST(r, f, x, m) (errno = 0, msg = #f, ((r) = (f)) == (x) || (t_error("%s failed (" m ")\n", #f, r, x), 0))

#define TEST2(r, f, x, m) (((r) = (f)) == (x) || (t_error("%s failed (" m ")\n", msg, r, x), 0))

/**
 * @tc.name      : wcstoll_0100
 * @tc.desc      : Convert wide string "2147483647" to long long type
 * @tc.level     : Level 0
 */
void wcstoll_0100(void)
{
    long long ll;
    char *msg = "";
    TEST(ll, wcstoll(L"2147483647", 0, 0), 2147483647LL, "expect signed %lld != %lld");
}

/**
 * @tc.name      : wcstoll_0200
 * @tc.desc      : Convert wide string "10" to long long type
 * @tc.level     : Level 0
 */
void wcstoll_0200(void)
{
    long long ll;
    char *msg = "";
    TEST(ll, wcstoll(L"10", 0, 0), 10LL, "expect signed %lld != %lld");
}

/**
 * @tc.name      : wcstoll_0300
 * @tc.desc      : Convert wide string "z" to long long type
 * @tc.level     : Level 1
 */
void wcstoll_0300(void)
{
    long long ll;
    char *msg = "";
    TEST(ll, wcstoll(L"z", 0, 36), 35LL, "%lld != %lld");
}

/**
 * @tc.name      : wcstoll_0400
 * @tc.desc      : Convert wide string "00010010001101000101011001111000" to long long type
 * @tc.level     : Level 1
 */
void wcstoll_0400(void)
{
    long long ll;
    char *msg = "";
    TEST(ll, wcstoll(L"00010010001101000101011001111000", 0, 2), 0x12345678, "%lld != %lld");
}

/**
 * @tc.name      : wcstoll_0500
 * @tc.desc      : Convert wide string "0xz" to long long type
 * @tc.level     : Level 1
 */
void wcstoll_0500(void)
{
    int i;
    long long ll;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ll, wcstoll(s = L"0xz", &c, 16), 0, "%lld != %lld");
    TEST2(i, c - s, 1, "wrong final position %lld != %lld");
}

/**
 * @tc.name      : wcstoll_0600
 * @tc.desc      : Convert wide string "0x1234" to long long type
 * @tc.level     : Level 1
 */
void wcstoll_0600(void)
{
    int i;
    long long ll;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ll, wcstoll(s = L"0x1234", &c, 16), 0x1234, "%lld != %ld");
    TEST2(i, c - s, 6, "wrong final position %lld != %lld");
}

/**
 * @tc.name      : wcstoll_0700
 * @tc.desc      : Convert wide string "9223372036854775808" to long long type exceeds the maximum value of the type
 * @tc.level     : Level 2
 */
void wcstoll_0700(void)
{
    int i;
    long long ll;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ll, wcstoll(s = L"9223372036854775808", &c, 0), 9223372036854775807LL, "uncaught overflow %lld != %lld");
    TEST2(i, c - s, 19, "wrong final position %d != %d");
    TEST2(i, errno, ERANGE, "missing errno %d != %d");
}

/**
 * @tc.name      : wcstoll_0800
 * @tc.desc      : Convert wide string "-9223372036854775809" to long long type exceeds the maximum value of the type
 * @tc.level     : Level 2
 */
void wcstoll_0800(void)
{
    int i;
    long long ll;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ll, wcstoll(s = L"-9223372036854775809", &c, 0), -9223372036854775807LL - 1, "overflow %lld != %lld");
    TEST2(i, c - s, 20, "wrong final position %d != %d");
    TEST2(i, errno, ERANGE, "missing errno %d != %d");
}

int main(int argc, char *argv[])
{
    wcstoll_0100();
    wcstoll_0200();
    wcstoll_0300();
    wcstoll_0400();
    wcstoll_0500();
    wcstoll_0600();
    wcstoll_0700();
    wcstoll_0800();
    return t_status;
}
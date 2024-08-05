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
 * @tc.name      : wcstoull_0100
 * @tc.desc      : Convert wide string to unsigned long long type based on decimal
 * @tc.level     : Level 0
 */
void wcstoull_0100(void)
{
    unsigned long long ull;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ull, wcstoull(s = L"250068492", &c, 10), 250068492ULL, "expect unsigned %llu != %llu");
}

/**
 * @tc.name      : wcstoull_0200
 * @tc.desc      : Convert wide string to unsigned long long type based on hexadecimal
 * @tc.level     : Level 1
 */
void wcstoull_0200(void)
{
    unsigned long long ull;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ull, wcstoull(s = L"7b06af00", &c, 16), 2064035584ULL, "expect unsigned %llu != %llu");
}

/**
 * @tc.name      : wcstoull_0300
 * @tc.desc      : Convert wide string to unsigned long long type based on binary
 * @tc.level     : Level 1
 */
void wcstoull_0300(void)
{
    unsigned long long ull;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ull, wcstoull(s = L"1100011011110101010001100000", &c, 2), 208622688ULL, "expect unsigned %llu != %llu");
}

/**
 * @tc.name      : wcstoull_0400
 * @tc.desc      : Convert wide string to unsigned long long type based on parameter 'base' equal to 0
 * @tc.level     : Level 1
 */
void wcstoull_0400(void)
{
    unsigned long long ull;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ull, wcstoull(s = L"0x6fffff", &c, 0), 7340031ULL, "expect unsigned %llu != %llu");
}

/**
 * @tc.name      : wcstoull_0500
 * @tc.desc      : Convert wide string to unsigned long long type exceeds the maximum value of the type
 * @tc.level     : Level 2
 */
void wcstoull_0500(void)
{
    int i;
    unsigned long long ull;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ull, wcstoull(s = L"18446744073709551616", &c, 0), 18446744073709551615ULL, "uncaught overflow %llu != %llu");
    TEST2(i, c - s, 20, "wrong final position %d != %d");
    TEST2(i, errno, ERANGE, "missing errno %d != %d");
}

/**
 * @tc.name      : wcstoull_0600
 * @tc.desc      : The converted result value -1 is invalid
 * @tc.level     : Level 2
 */
void wcstoull_0600(void)
{
    int i;
    unsigned long long ull;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ull, wcstoull(s = L"-1", &c, 0), -1ULL, "rejected negative %llu != %llu");
    TEST2(i, c - s, 2, "wrong final position %d != %d");
    TEST2(i, errno, 0, "spurious errno %d != %d");
}

/**
 * @tc.name      : wcstoull_0700
 * @tc.desc      : The converted result value -2 is invalid
 * @tc.level     : Level 2
 */
void wcstoull_0700(void)
{
    int i;
    unsigned long long ull;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ull, wcstoull(s = L"-2", &c, 0), -2ULL, "rejected negative %llu != %llu");
    TEST2(i, c - s, 2, "wrong final position %d != %d");
    TEST2(i, errno, 0, "spurious errno %d != %d");
}

/**
 * @tc.name      : wcstoull_0800
 * @tc.desc      : The converted result value -9223372036854775808 is invalid
 * @tc.level     : Level 2
 */
void wcstoull_0800(void)
{
    int i;
    unsigned long long ull;
    char *msg = "";
    wchar_t *s, *c;
    TEST(ull, wcstoull(s = L"-9223372036854775808", &c, 0), -9223372036854775808ULL, "rejected negative %llu != %llu");
    TEST2(i, c - s, 20, "wrong final position %d != %d");
    TEST2(i, errno, 0, "spurious errno %d != %d");
}

int main(int argc, char *argv[])
{
    wcstoull_0100();
    wcstoull_0200();
    wcstoull_0300();
    wcstoull_0400();
    wcstoull_0500();
    wcstoull_0600();
    wcstoull_0700();
    wcstoull_0800();
    return t_status;
}
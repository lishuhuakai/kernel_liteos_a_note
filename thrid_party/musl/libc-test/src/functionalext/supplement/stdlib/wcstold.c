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
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include "math.h"
#include "test.h"

#define EXPECT_LONG_DOUBLE_EQ(a, b)                                        \
    do {                                                                   \
        if (!(fabsl(a - b) < LDBL_EPSILON))                                \
            t_error("%s failed: %f is not equal to %f\n", __func__, a, b); \
    } while (0)

/**
 * @tc.name      : wcstold_0100
 * @tc.desc      : Convert wide string to long double type
 * @tc.level     : Level 0
 */
void wcstold_0100(void)
{
    wchar_t str0[] = L"3.14wcstold";
    wchar_t *end = NULL;
    const long double target = 3.14L;
    wchar_t str1[] = L"wcstold";
    long double val = wcstold(str0, &end);
    EXPECT_LONG_DOUBLE_EQ(val, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstold_0200
 * @tc.desc      : Convert wide string to long double type with no end string characters
 * @tc.level     : Level 1
 */
void wcstold_0200(void)
{
    wchar_t str[] = L"3.14";
    wchar_t *end = NULL;
    const long double target = 3.14L;
    long double val = wcstold(str, &end);
    EXPECT_LONG_DOUBLE_EQ(val, target);
}

/**
 * @tc.name      : wcstold_0300
 * @tc.desc      : Convert wide string to long double type with whitespace present at the beginning
 * @tc.level     : Level 1
 */
void wcstold_0300(void)
{
    wchar_t str[] = L"          3.14";
    wchar_t *end = NULL;
    const long double target = 3.14L;
    long double val = wcstold(str, &end);
    EXPECT_LONG_DOUBLE_EQ(val, target);
}

/**
 * @tc.name      : wcstold_0400
 * @tc.desc      : Convert wide string to long double type with string characters at the beginning
 * @tc.level     : Level 2
 */
void wcstold_0400(void)
{
    wchar_t str[] = L"wcstold3.14";
    wchar_t *end = NULL;
    const long double target = 3.14L;
    long double val = wcstold(str, &end);
    if (val != 0) {
        t_error("%s invalid conversion", __func__);
    }
}

/**
 * @tc.name      : wcstold_0500
 * @tc.desc      : Convert wide string to negative number of type long double
 * @tc.level     : Level 1
 */
void wcstold_0500(void)
{
    wchar_t str[] = L"-3.14";
    wchar_t *end = NULL;
    const long double target = -3.14L;
    long double val = wcstold(str, &end);
    EXPECT_LONG_DOUBLE_EQ(val, target);
}

/**
 * @tc.name      : wcstold_0600
 * @tc.desc      : The converted value is greater than long double max
 * @tc.level     : Level 2
 */
void wcstold_0600(void)
{
    errno = 0;
    wchar_t str[] = L"1.18973e+49322";
    wchar_t *end = NULL;
    long double val = wcstold(str, &end);
    if (errno != ERANGE) {
        t_error("%s errno is not set\n", __func__);
    }
    if (val != HUGE_VALL) {
        t_error("%s val is not equal to inf\n", __func__);
    }
}

int main(int argc, char *argv[])
{
    wcstold_0100();
    wcstold_0200();
    wcstold_0300();
    wcstold_0400();
    wcstold_0500();
    wcstold_0600();
    return t_status;
}
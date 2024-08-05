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

#define EXPECT_DOUBLE_EQ(a, b)                                             \
    do {                                                                   \
        if (!(fabs(a - b) < DBL_EPSILON))                                  \
            t_error("%s failed: %f is not equal to %f\n", __func__, a, b); \
    } while (0)

/**
 * @tc.name      : wcstod_0100
 * @tc.desc      : Convert wide string to dobule type
 * @tc.level     : Level 0
 */
void wcstod_0100(void)
{
    wchar_t str0[] = L"3.14wcstod";
    wchar_t *end = NULL;
    const double target = 3.14;
    wchar_t str1[] = L"wcstod";
    double val = wcstod(str0, &end);
    EXPECT_DOUBLE_EQ(val, target);
    if (wcscmp(end, str1)) {
        t_error("%s the result of comparing two strings should be equal", __func__);
    }
}

/**
 * @tc.name      : wcstod_0200
 * @tc.desc      : Convert wide string to dobule type with no end string characters
 * @tc.level     : Level 1
 */
void wcstod_0200(void)
{
    wchar_t str[] = L"3.14";
    wchar_t *end = NULL;
    const double target = 3.14;
    double val = wcstod(str, &end);
    EXPECT_DOUBLE_EQ(val, target);
}

/**
 * @tc.name      : wcstod_0300
 * @tc.desc      : Convert wide string to dobule type with whitespace present at the beginning
 * @tc.level     : Level 1
 */
void wcstod_0300(void)
{
    wchar_t str[] = L"          3.14";
    wchar_t *end = NULL;
    const double target = 3.14;
    double val = wcstod(str, &end);
    EXPECT_DOUBLE_EQ(val, target);
}

/**
 * @tc.name      : wcstod_0400
 * @tc.desc      : Convert wide string to dobule type with string characters at the beginning
 * @tc.level     : Level 2
 */
void wcstod_0400(void)
{
    wchar_t str[] = L"wcstod3.14";
    wchar_t *end = NULL;
    const double target = 3.14;
    double val = wcstod(str, &end);
    if (val != 0) {
        t_error("%s invalid conversion", __func__);
    }
}

/**
 * @tc.name      : wcstod_0500
 * @tc.desc      : Convert wide string to negative number of type double
 * @tc.level     : Level 1
 */
void wcstod_0500(void)
{
    wchar_t str[] = L"-3.14";
    wchar_t *end = NULL;
    const double target = -3.14;
    double val = wcstod(str, &end);
    EXPECT_DOUBLE_EQ(val, target);
}

/**
 * @tc.name      : wcstod_0600
 * @tc.desc      : Convert the hexadecimal string in the wide string to double type
 * @tc.level     : Level 1
 */
void wcstod_0600(void)
{
    wchar_t str[] = L"0X1.BC70A3D70A3D7P+6";
    wchar_t *end = NULL;
    double val = wcstod(str, &end);
    const double target = 111.11;
    EXPECT_DOUBLE_EQ(val, target);
}

/**
 * @tc.name      : wcstod_0700
 * @tc.desc      : The converted value is greater than double max
 * @tc.level     : Level 2
 */
void wcstod_0700(void)
{
    wchar_t str[] = L"1.18973e+4932";
    wchar_t *end = NULL;
    double val = wcstod(str, &end);
    if (errno != ERANGE) {
        t_error("%s errno is not set", __func__);
    }
    if (val != INFINITY) {
        t_error("%s val is not equal to inf", __func__);
    }
}

int main(int argc, char *argv[])
{
    wcstod_0100();
    wcstod_0200();
    wcstod_0300();
    wcstod_0400();
    wcstod_0500();
    wcstod_0600();
    wcstod_0700();
    return t_status;
}
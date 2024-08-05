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

#include <stdlib.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const int32_t ZERO = 0;
const int32_t NUMBER = 123456;
const int32_t NUM = 123;
const int32_t NUMB = 1234;
const int32_t NU = 12;
const int32_t BURDENNUM = -2147483648;

/**
 * @tc.name      : atoi_0100
 * @tc.desc      : Verify that there are and only numbers (starting with non-zero digits) strings are
 *                 converted to integers
 * @tc.level     : Level 0
 */
void atoi_0100(void)
{
    char str[] = "123456";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0100", fptr, NUMBER);
}

/**
 * @tc.name      : atoi_0200
 * @tc.desc      : Verify that there are and only numbers (starting with a 0) string is converted to an integer
 * @tc.level     : Level 0
 */
void atoi_0200(void)
{
    char str[] = "000123456";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0200", fptr, NUMBER);
}

/**
 * @tc.name      : atoi_0300
 * @tc.desc      : Verify that spaces are at the beginning of the string string is converted to an integer
 * @tc.level     : Level 1
 */
void atoi_0300(void)
{
    char str[] = " 123456";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0300", fptr, NUMBER);
}

/**
 * @tc.name      : atoi_0400
 * @tc.desc      : Verify that spaces are in the middle of the string and convert the string to an integer
 * @tc.level     : Level 1
 */
void atoi_0400(void)
{
    char str[] = "123  456";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0400", fptr, NUM);
}

/**
 * @tc.name      : atoi_0500
 * @tc.desc      : Verify that string conversion to integer (- at the beginning of the number)
 * @tc.level     : Level 1
 */
void atoi_0500(void)
{
    char str[] = "-123456";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0500", fptr, -NUMBER);
}

/**
 * @tc.name      : atoi_0600
 * @tc.desc      : Verify that string conversion to integer (- in the middle of the number)
 * @tc.level     : Level 1
 */
void atoi_0600(void)
{
    char str[] = "1234-56";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0600", fptr, NUMB);
}

/**
 * @tc.name      : atoi_0700
 * @tc.desc      : Verify that string conversion to integer (+ at the beginning of the number)
 * @tc.level     : Level 1
 */
void atoi_0700(void)
{
    char str[] = "+123456";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0700", fptr, NUMBER);
}

/**
 * @tc.name      : atoi_0800
 * @tc.desc      : Verify that string conversion to integer (+ in the middle of the number)
 * @tc.level     : Level 1
 */
void atoi_0800(void)
{
    char str[] = "1234+56";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0800", fptr, NUMB);
}

/**
 * @tc.name      : atoi_0900
 * @tc.desc      : Verify that string conversion to integer (English characters are at the beginning)
 * @tc.level     : Level 2
 */
void atoi_0900(void)
{
    char str[] = "a123456";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_0900", fptr, ZERO);
}

/**
 * @tc.name      : atoi_1000
 * @tc.desc      : Verify that string conversion to integer (English characters are in the middle)
 * @tc.level     : Level 2
 */
void atoi_1000(void)
{
    char str[] = "12b3456";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_1000", fptr, NU);
}

/**
 * @tc.name      : atoi_1100
 * @tc.desc      : Verify that string conversion to integer (parameter invalid NULL)
 * @tc.level     : Level 2
 */
void atoi_1100(void)
{
    char str[] = "NULL";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_1100", fptr, ZERO);
}

/**
 * @tc.name      : atoi_1200
 * @tc.desc      : Verify that string conversion to integer (parameter invalid "")
 * @tc.level     : Level 2
 */
void atoi_1200(void)
{
    char str[] = "";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_1200", fptr, ZERO);
}

/**
 * @tc.name      : atoi_1300
 * @tc.desc      : Verify that string conversion to integer (parameter exceeds max value)
 * @tc.level     : Level 2
 */
void atoi_1300(void)
{
    char str[] = "2147483648";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_1300", fptr, BURDENNUM);
}

/**
 * @tc.name      : atoi_1400
 * @tc.desc      : erify that string conversion to integer (parameter less than minimum value)
 * @tc.level     : Level 2
 */
void atoi_1400(void)
{
    char str[] = "-2147483648";
    int32_t fptr = atoi(str);
    EXPECT_EQ("atoi_1400", fptr, BURDENNUM);
}

TEST_FUN G_Fun_Array[] = {
    atoi_0100,
    atoi_0200,
    atoi_0300,
    atoi_0400,
    atoi_0500,
    atoi_0600,
    atoi_0700,
    atoi_0800,
    atoi_0900,
    atoi_1000,
    atoi_1100,
    atoi_1200,
    atoi_1300,
    atoi_1400,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
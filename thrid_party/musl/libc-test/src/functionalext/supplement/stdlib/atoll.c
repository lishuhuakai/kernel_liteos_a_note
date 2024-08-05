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
#include <ctype.h>
#include "functionalext.h"

long long int result = 123456789012345;
int resultA = 123456;
int resultC = -123456;
int resultD = 1234567890;
int resultE = 1234;
int successfully = 0;
int resultG = 12;

/**
 * @tc.name      : atoll_0100
 * @tc.desc      : Verify that the string can be converted to an integer (parameter is 123456789012345)
 * @tc.level     : Level 0
 */
void atoll_0100(void)
{
    long long int num;
    char str[] = "123456789012345";
    num = atoll(str);
    EXPECT_EQ("atoll_0100", num, result);
}

/**
 * @tc.name      : atoll_0200
 * @tc.desc      : Verify that the string can be converted to an integer (parameter is 000123456)
 * @tc.level     : Level 0
 */
void atoll_0200(void)
{
    long long int num;
    char str[] = "000123456";
    num = atoll(str);
    EXPECT_EQ("atoll_0200", num, resultA);
}

/**
 * @tc.name      : atoll_0300
 * @tc.desc      : Verify that the string can be converted to an integer (parameter is  123456)
 * @tc.level     : Level 1
 */
void atoll_0300(void)
{
    long long int num;
    char str[] = " 123456";
    num = atoll(str);
    EXPECT_EQ("atoll_0300", num, resultA);
}

/**
 * @tc.name      : atoll_0400
 * @tc.desc      : Verify that the string can be converted to an integer (parameter is 123 456)
 * @tc.level     : Level 1
 */
void atoll_0400(void)
{
    long long int num;
    char str[] = "123 456";
    num = atoll(str);
    EXPECT_EQ("atoll_0400", num, 123);
}

/**
 * @tc.name      : atoll_0500
 * @tc.desc      : Verify that the string can be converted to an integer (parameter is -123456)
 * @tc.level     : Level 1
 */
void atoll_0500(void)
{
    long long int num;
    char str[] = "-123456";
    num = atoll(str);
    EXPECT_EQ("atoll_0500", num, resultC);
}

/**
 * @tc.name      : atoll_0600
 * @tc.desc      : Verify that the string can be converted to an integer (parameter is 1234567890-56)
 * @tc.level     : Level 1
 */
void atoll_0600(void)
{
    long long int num;
    char str[] = "1234567890-56";
    num = atoll(str);
    EXPECT_EQ("atoll_0600", num, resultD);
}

/**
 * @tc.name      : atoll_0700
 * @tc.desc      : Verify that the string can be converted to an integer (parameter is +123456)
 * @tc.level     : Level 1
 */
void atoll_0700(void)
{
    long long int num;
    char str[] = "+123456";
    num = atoll(str);
    EXPECT_EQ("atoll_0700", num, resultA);
}

/**
 * @tc.name      : atoll_0800
 * @tc.desc      : Verify that the string can be converted to an integer (parameter is 1234+56)
 * @tc.level     : Level 1
 */
void atoll_0800(void)
{
    long long int num;
    char str[] = "1234+56";
    num = atoll(str);
    EXPECT_EQ("atoll_0800", num, resultE);
}

/**
 * @tc.name      : atoll_0900
 * @tc.desc      : Verify that cannot convert string to integer (parameter is a123456)
 * @tc.level     : Level 2
 */
void atoll_0900(void)
{
    long long int num;
    char str[] = "a123456";
    num = atoll(str);
    EXPECT_EQ("atoll_0900", num, successfully);
}

/**
 * @tc.name      : atoll_1000
 * @tc.desc      : Verify that cannot convert string to integer (parameter is 12b3456789012345)
 * @tc.level     : Level 2
 */
void atoll_1000(void)
{
    long long int num;
    char str[] = "12b3456789012345";
    num = atoll(str);
    EXPECT_EQ("atoll_1000", num, resultG);
}

/**
 * @tc.name      : atoll_1100
 * @tc.desc      : Verify that cannot convert string to integer (parameter is NULL)
 * @tc.level     : Level 2
 */
void atoll_1100(void)
{
    long long int num;
    char str[] = "NULL";
    num = atoll(str);
    EXPECT_EQ("atoll_1100", num, successfully);
}

/**
 * @tc.name      : atoll_1200
 * @tc.desc      : Verify that cannot convert string to integer (parameter is “”)
 * @tc.level     : Level 2
 */
void atoll_1200(void)
{
    long long int num;
    char str[] = "";
    num = atoll(str);
    EXPECT_EQ("atoll_1200", num, successfully);
}

int main(int argc, char *argv[])
{
    atoll_0100();
    atoll_0200();
    atoll_0300();
    atoll_0400();
    atoll_0500();
    atoll_0600();
    atoll_0700();
    atoll_0800();
    atoll_0900();
    atoll_1000();
    atoll_1100();
    atoll_1200();

    return t_status;
}
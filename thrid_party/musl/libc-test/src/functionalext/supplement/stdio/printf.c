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

#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : printf_0100
 * @tc.desc      : Verify the output data in the specified format (the format character is d)
 * @tc.level     : Level 0
 */
void printf_0100(void)
{
    int num = 6;
    int result = printf("%d\n", num);
    EXPECT_TRUE("printf_0100", result > 0);
}

/**
 * @tc.name      : printf_0200
 * @tc.desc      : Verify the output data in the specified format (the format character is o)
 * @tc.level     : Level 0
 */
void printf_0200(void)
{
    int num = 6;
    int result = printf("%o\n", num);
    EXPECT_TRUE("printf_0200", result > 0);
}

/**
 * @tc.name      : printf_0300
 * @tc.desc      : Verify the output data in the specified format (the format character is x,X)
 * @tc.level     : Level 0
 */
void printf_0300(void)
{
    int num = 6;
    int result = printf("%x\n", num);
    EXPECT_TRUE("printf_0300", result > 0);

    result = printf("%X\n", num);
    EXPECT_TRUE("printf_0300", result > 0);
}

/**
 * @tc.name      : printf_0400
 * @tc.desc      : Verify the output data in the specified format (the format character is u)
 * @tc.level     : Level 0
 */
void printf_0400(void)
{
    int num = 6;
    int result = printf("%u\n", num);
    EXPECT_TRUE("printf_0400", result > 0);
}

/**
 * @tc.name      : printf_0500
 * @tc.desc      : Verify the output data in the specified format (the format character is f)
 * @tc.level     : Level 0
 */
void printf_0500(void)
{
    int num = 6;
    int result = printf("%f\n", num);
    EXPECT_TRUE("printf_0500", result > 0);
}

/**
 * @tc.name      : printf_0600
 * @tc.desc      : Verify the output data in the specified format (the format character is e,E)
 * @tc.level     : Level 0
 */
void printf_0600(void)
{
    int num = 6;
    int result = printf("%e\n", num);
    EXPECT_TRUE("printf_0600", result > 0);

    result = printf("%E\n", num);
    EXPECT_TRUE("printf_0600", result > 0);
}

/**
 * @tc.name      : printf_0700
 * @tc.desc      : Verify the output data in the specified format (the format character is g,G)
 * @tc.level     : Level 0
 */
void printf_0700(void)
{
    int num = 6;
    int result = printf("%g\n", num);
    EXPECT_TRUE("printf_0700", result > 0);

    result = printf("%G\n", num);
    EXPECT_TRUE("printf_0700", result > 0);
}

/**
 * @tc.name      : printf_0800
 * @tc.desc      : Verify the output data in the specified format (the format character is c)
 * @tc.level     : Level 0
 */
void printf_0800(void)
{
    char ch = 'a';
    int result = printf("%c\n", ch);
    EXPECT_TRUE("printf_0800", result > 0);
}

/**
 * @tc.name      : printf_0900
 * @tc.desc      : Verify the output data in the specified format (the format character is s)
 * @tc.level     : Level 0
 */
void printf_0900(void)
{
    char num[] = "test";
    int result = printf("%s\n", num);
    EXPECT_TRUE("printf_0900", result > 0);
}

int main(int argc, char *argv[])
{
    printf_0100();
    printf_0200();
    printf_0300();
    printf_0400();
    printf_0500();
    printf_0600();
    printf_0700();
    printf_0800();
    printf_0900();

    return t_status;
}
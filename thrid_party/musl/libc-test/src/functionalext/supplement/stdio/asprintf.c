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

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : asprintf_0100
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0100(void)
{
    char *buf = "d";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0100", n > 0);
}

/**
 * @tc.name      : asprintf_0200
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0200(void)
{
    char *buf = "o";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0200", n > 0);
}

/**
 * @tc.name      : asprintf_0300
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0300(void)
{
    char *buf = "x,X";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0300", n > 0);
}

/**
 * @tc.name      : asprintf_0400
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0400(void)
{
    char *buf = "u";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0400", n > 0);
}

/**
 * @tc.name      : asprintf_0500
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0500(void)
{
    char *buf = "f";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0500", n > 0);
}

/**
 * @tc.name      : asprintf_0600
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0600(void)
{
    char *buf = "e,E";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0600", n > 0);
}

/**
 * @tc.name      : asprintf_0700
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0700(void)
{
    char *buf = "g,G";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0700", n > 0);
}

/**
 * @tc.name      : asprintf_0800
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0800(void)
{
    char *buf = "c";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0800", n > 0);
}

/**
 * @tc.name      : asprintf_0900
 * @tc.desc      : The parameters are valid and the data can be output according to the format
 * @tc.level     : Level 0
 */
void asprintf_0900(void)
{
    char *buf = "s";
    char *testStr = NULL;
    int n;
    n = asprintf(&testStr, "%s", buf);
    free(testStr);
    testStr = NULL;
    EXPECT_TRUE("asprintf_0900", n > 0);
}

TEST_FUN G_Fun_Array[] = {
    asprintf_0100,
    asprintf_0200,
    asprintf_0300,
    asprintf_0400,
    asprintf_0500,
    asprintf_0600,
    asprintf_0700,
    asprintf_0800,
    asprintf_0900,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
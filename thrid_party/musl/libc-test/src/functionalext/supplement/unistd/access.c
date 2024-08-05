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
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

const int SUCCESS = 0;
const int FAILED = -1;

/**
 * @tc.name      : access_0100
 * @tc.desc      : The parameter filename is the specified file and exists,
 *                 the parameter amode is F_OK,function returns successfully.
 * @tc.level     : Level 0
 */
void access_0100(void)
{
    const char *ptr = "accesstest.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("access_0100", fptr, NULL);
    int isExist = access(ptr, F_OK);
    EXPECT_EQ("access_0100", isExist, SUCCESS);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : access_0200
 * @tc.desc      : The parameter filename is the specified directory and exists,
 *                 the parameter amode is F_OK,function returns SUCCESSfully.
 * @tc.level     : Level 0
 */
void access_0200(void)
{
    char passbuff[90];
    const char *ptr = "accesstest.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("access_0200", fptr, NULL);
    getcwd(passbuff, sizeof(passbuff));
    int isExist = access(passbuff, F_OK);
    EXPECT_EQ("access_0200", isExist, SUCCESS);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : access_0300
 * @tc.desc      : The parameter filename is the specified file and not exists,
 *                 the parameter amode is F_OK,function returns failure.
 * @tc.level     : Level 2
 */
void access_0300(void)
{
    const char *ptr = "noaccesstest.txt";
    int isExist = access(ptr, F_OK);
    EXPECT_EQ("access_0300", isExist, FAILED);
    remove(ptr);
}

/**
 * @tc.name      : access_0400
 * @tc.desc      : The parameter filename is the specified directory and not exists,
 *                 the parameter amode is F_OK,function returns failure.
 * @tc.level     : Level 2
 */
void access_0400(void)
{
    const char *ptr = "./noaccesstest";
    int isExist = access(ptr, F_OK);
    EXPECT_EQ("access_0400", isExist, FAILED);
    remove(ptr);
}

/**
 * @tc.name      : access_0500
 * @tc.desc      : The file attribute specified by the parameter filename is readable,
 *                 the parameter amode is R_OK,function returns successfully.
 * @tc.level     : Level 0
 */
void access_0500(void)
{
    const char *ptr = "accesstest.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("access_0500", fptr, NULL);
    int isRead = access(ptr, R_OK);
    EXPECT_EQ("access_0500", isRead, SUCCESS);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : access_0600
 * @tc.desc      : The file attribute specified by the parameter filename is writable,
 *                 the parameter amode is W_OK,function returns successfully.
 * @tc.level     : Level 0
 */
void access_0600(void)
{
    const char *ptr = "accesstest.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("access_0600", fptr, NULL);
    int isWrite = access(ptr, W_OK);
    EXPECT_EQ("access_0600", isWrite, SUCCESS);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : access_0700
 * @tc.desc      : The file attribute specified by the parameter filename is executable,
 *                 the parameter amode is W_OK,function returns successfully.
 * @tc.level     : Level 0
 */
void access_0700(void)
{
    const char *ptr = "accesstest.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("access_0700", fptr, NULL);
    system("chmod 777 ./accesstest.txt");
    int isExecute = access(ptr, X_OK);
    EXPECT_EQ("access_0700", isExecute, SUCCESS);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : access_0800
 * @tc.desc      : The file attribute specified by the parameter filename is not executable,
 *                 the parameter amode is R_OK,function returns failure.
 * @tc.level     : Level 2
 */
void access_0800(void)
{
    const char *ptr = "accesstest.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_PTRNE("access_0800", fptr, NULL);
    int isExecute = access(ptr, X_OK);
    EXPECT_EQ("access_0800", isExecute, FAILED);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

TEST_FUN G_Fun_Array[] = {
    access_0100,
    access_0200,
    access_0300,
    access_0400,
    access_0500,
    access_0600,
    access_0700,
    access_0800,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }
    return t_status;
}
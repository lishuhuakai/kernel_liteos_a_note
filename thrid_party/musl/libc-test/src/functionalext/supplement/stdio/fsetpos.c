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
#include "functionalext.h"

const int SUCCESS = 0;
const int SIZE = 20;
const int HALF_SIZE = 10;
const int END_SIZE = 0;

/**
 * @tc.name      : fsetpos_0100
 * @tc.desc      : Each parameter is valid, and the file pointer is set to the beginning of the file.
 * @tc.level     : Level 0
 */
void fsetpos_0100()
{
    char str[100] = {0};
    const char *ptr = "test.txt";
    const char *wstring = "This is a test case!";
    long long a = 0;
    fpos_t pos;
    pos = (fpos_t)a;
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_TRUE("fsetpos_0100", fptr != NULL);
    fwrite(wstring, sizeof(char), strlen(wstring), fptr);
    int result = fsetpos(fptr, &pos);
    EXPECT_EQ("fsetpos_0100", result, SUCCESS);
    int rsize = fread(str, sizeof(char), 100, fptr);
    EXPECT_EQ("fsetpos_0100", rsize, SIZE);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : fsetpos_0200
 * @tc.desc      : Each parameter is valid, and the file pointer is set to the middle of the file.
 * @tc.level     : Level 0
 */
void fsetpos_0200()
{
    char str[100] = {0};
    const char *ptr = "test.txt";
    const char *wstring = "This is a test case!";
    fpos_t pos;
    long long a = 10;
    pos = (fpos_t)a;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wstring, sizeof(char), strlen(wstring), fptr);
    int data = fsetpos(fptr, &pos);
    EXPECT_EQ("fsetpos_0200", data, SUCCESS);
    int rsize = fread(str, 1, 100, fptr);
    EXPECT_EQ("fsetpos_0200", rsize, HALF_SIZE);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
    wstring = NULL;
}

/**
 * @tc.name      : fsetpos_0300
 * @tc.desc      : Each parameter is valid, and the file pointer is set to the end of the file.
 * @tc.level     : Level 0
 */
void fsetpos_0300()
{
    char str[100] = {0};
    const char *ptr = "test.txt";
    const char *wstring = "This is a test case!";
    fpos_t pos;
    long long a = 20;
    pos = (fpos_t)a;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wstring, sizeof(char), strlen(wstring), fptr);
    int data = fsetpos(fptr, &pos);
    EXPECT_EQ("fsetpos_0300", data, SUCCESS);
    int rsize = fread(str, 1, 100, fptr);
    EXPECT_EQ("fsetpos_0300", rsize, END_SIZE);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
    wstring = NULL;
}

/**
 * @tc.name      : fsetpos_0400
 * @tc.desc      : The pos parameter is valid, and the file pointer cannot be set.
 * @tc.level     : Level 2
 */
void fsetpos_0400()
{
    char str[100] = {0};
    const char *ptr = "test.txt";
    const char *wrstring = "This is a test case!";
    fpos_t pos;
    long long a = -1;
    pos = (fpos_t)a;
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    int data = fsetpos(fptr, &pos);
    EXPECT_NE("fsetpos_0400", data, SUCCESS);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
    wrstring = NULL;
}

int main(int argc, char *argv[])
{
    fsetpos_0100();
    fsetpos_0200();
    fsetpos_0300();
    fsetpos_0400();
    return t_status;
}
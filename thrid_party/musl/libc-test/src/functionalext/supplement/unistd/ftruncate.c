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

#include <sys/stat.h>
#include <unistd.h>
#include "functionalext.h"

const int SUCCESS = 0;
const int FAILED = -1;
const int FILE_ZERO = 0;
const int FILE_FIRST = 10;
const int FILE_SECOND = 100;

/**
 * @tc.name      : ftruncate_0100
 * @tc.desc      : The parameter length is 0, which can clear the file content.
 * @tc.level     : Level 0
 */
void ftruncate_0100(void)
{
    const char *ptr = "test.txt";
    FILE *fptr = fopen(ptr, "w");
    struct stat statbuff;
    fprintf(fptr, "%s", "this is a sample!");
    int freturn = ftruncate(fileno(fptr), 0);
    EXPECT_EQ("ftruncate_0100", freturn, SUCCESS);
    stat(ptr, &statbuff);
    EXPECT_EQ("ftruncate_0100", statbuff.st_size, FILE_ZERO);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : ftruncate_0200
 * @tc.desc      : The parameter length is greater than 0 and less than the file size,
 *                 which can clear the file content.
 * @tc.level     : Level 0
 */
void ftruncate_0200(void)
{
    const char *ptr = "test.txt";
    FILE *fptr = fopen(ptr, "w");
    struct stat statbuff;
    fprintf(fptr, "%s", "this is a sample!");
    int freturn = ftruncate(fileno(fptr), 10);
    EXPECT_EQ("ftruncate_0200", freturn, SUCCESS);
    stat(ptr, &statbuff);
    EXPECT_EQ("ftruncate_0200", (int)statbuff.st_size, FILE_FIRST);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : ftruncate_0300
 * @tc.desc      : The parameter length is greater than the file size,which can clear the file content.
 * @tc.level     : Level 0
 */
void ftruncate_0300(void)
{
    const char *ptr = "test.txt";
    FILE *fptr = fopen(ptr, "w");
    struct stat statbuff;
    fprintf(fptr, "%s", "this is a sample!");
    int freturn = ftruncate(fileno(fptr), 100);
    EXPECT_EQ("ftruncate_0300", freturn, SUCCESS);
    stat(ptr, &statbuff);
    EXPECT_EQ("ftruncate_0300", (int)statbuff.st_size, FILE_SECOND);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : ftruncate_0400
 * @tc.desc      : The fd parameter is NULL,the file content cannot be cleared.
 * @tc.level     : Level 2
 */
void ftruncate_0400(void)
{
    const char *ptr = "test.txt";
    FILE *fptr = fopen(ptr, "w");
    fprintf(fptr, "%s", "this is a sample!");
    int freturn = ftruncate(0, 10);
    EXPECT_EQ("ftruncate_0400", freturn, FAILED);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

int main(int argc, char *argv[])
{
    ftruncate_0100();
    ftruncate_0200();
    ftruncate_0300();
    ftruncate_0400();

    return t_status;
}
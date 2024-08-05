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

#include <stdbool.h>
#include "functionalext.h"

/**
 * @tc.name      : fgets_0100
 * @tc.desc      : The parameter n is greater than the number of characters,and all content can be read.
 * @tc.level     : Level 0
 */
void fgets_0100(void)
{
    bool successflag = false;
    char str[100];
    const char *ptr = "fgetstest.txt";
    char *wrstring = "this is a test\n";
    FILE *fptr = fopen(ptr, "wr+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fflush(fptr);
    fseek(fptr, 0L, SEEK_SET);
    char *content = fgets(str, 100, fptr);
    if (strcmp(content, "this is a test\n") == 0) {
        successflag = true;
    }
    EXPECT_TRUE("fgets_0100", successflag);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
    wrstring = NULL;
}

/**
 * @tc.name      : fgets_0200
 * @tc.desc      : The parameter n is greater than 2,and part of the content can be read.
 * @tc.level     : Level 0
 */
void fgets_0200(void)
{
    char str[5];
    bool successflag = false;
    const char *ptr = "fgetstest.txt";
    char *wrstring = "this is a test\n";
    FILE *fptr = fopen(ptr, "wr+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fflush(fptr);
    fseek(fptr, 0L, SEEK_SET);
    char *content = fgets(str, 5, fptr);
    if (strcmp(content, "this") == 0) {
        successflag = true;
    }
    EXPECT_TRUE("fgets_0200", successflag);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : fgets_0300
 * @tc.desc      : The parameter n is equal to 2,and part of the content can be read.
 * @tc.level     : Level 1
 */
void fgets_0300(void)
{
    char str[2];
    bool successflag = false;
    const char *ptr = "fgetstest.txt";
    char *wrstring = "this is a test\n";
    FILE *fptr = fopen(ptr, "wr+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fflush(fptr);
    fseek(fptr, 0L, SEEK_SET);
    char *content = fgets(str, 2, fptr);
    if (strcmp(content, "t") == 0) {
        successflag = true;
    }
    EXPECT_TRUE("fgets_0300", successflag);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : fgets_0400
 * @tc.desc      : The parameter n is equal to 1,and the content cannot be read.
 * @tc.level     : Level 2
 */
void fgets_0400(void)
{
    char str[2];
    bool successflag = false;
    const char *ptr = "fgetstest.txt";
    char *wrstring = "this is a test\n";
    FILE *fptr = fopen(ptr, "wr+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fflush(fptr);
    fseek(fptr, 0L, SEEK_SET);
    char *content = fgets(str, 1, fptr);
    if (*content == 0) {
        successflag = true;
    }
    EXPECT_TRUE("fgets_0400", successflag);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : fgets_0500
 * @tc.desc      : The parameter n is equal to 0,and the content cannot be read.
 * @tc.level     : Level 2
 */
void fgets_0500(void)
{
    char str[2];
    bool successflag = false;
    const char *ptr = "fgetstest.txt";
    char *wrstring = "this is a test\n";
    FILE *fptr = fopen(ptr, "wr+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fflush(fptr);
    fseek(fptr, 0L, SEEK_SET);
    char *content = fgets(str, 0, fptr);
    if (content == NULL) {
        successflag = true;
    }
    EXPECT_TRUE("fgets_0500", successflag);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : fgets_0600
 * @tc.desc      : Points to the end of the file,and the content cannot be read.
 * @tc.level     : Level 2
 */
void fgets_0600(void)
{
    char str[100];
    bool successflag = false;
    const char *ptr = "fgetstest.txt";
    char *wrstring = "this is a test\n";
    FILE *fptr = fopen(ptr, "wr+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fflush(fptr);
    fseek(fptr, 0L, SEEK_END);
    char *content = fgets(str, 100, fptr);
    if (content == NULL) {
        successflag = true;
    }
    EXPECT_TRUE("fgets_0600", successflag);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

int main(int argc, char *argv[])
{
    fgets_0100();
    fgets_0200();
    fgets_0300();
    fgets_0400();
    fgets_0500();
    fgets_0600();

    return t_status;
}
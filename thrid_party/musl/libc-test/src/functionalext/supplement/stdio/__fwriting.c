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
#include <stdio_ext.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "functionalext.h"
#include "filepath_util.h"

const int32_t NUM_ZERO = 0;

/**
 * @tc.name      : __fwriting_0100
 * @tc.desc      : Verify that the file stream is write-only
 * @tc.level     : Level 0
 */
void __fwriting_0100(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FREAD_TEST_TXT, ptr);
    FILE *fptr = fopen(ptr, "w");
    int result = __fwriting(fptr);
    EXPECT_NE("__fwriting_0100", result, NUM_ZERO);
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : __fwriting_0200
 * @tc.desc      : The last operation on the stream was a write operation
 * @tc.level     : Level 0
 */
void __fwriting_0200(void)
{
    const char *wrstring = "helloworld";
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FREAD_TEST_TXT, ptr);
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    int result = __fwriting(fptr);
    EXPECT_NE("__fwriting_0200", result, NUM_ZERO);
    fclose(fptr);
    remove(ptr);
}

/**
 * @tc.name      : __fwriting_0300
 * @tc.desc      : The last operation on the stream was not a write operation
 * @tc.level     : Level 2
 */
void __fwriting_0300(void)
{
    char abc[100] = {0};
    const char *wrstring = "helloworld";
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FREAD_TEST_TXT, ptr);
    FILE *fptr = fopen(ptr, "w+");
    fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    fseek(fptr, 0, SEEK_SET);
    int32_t rsize = fread(abc, 1, 10, fptr);
    int result = __fwriting(fptr);
    EXPECT_EQ("__fwriting_0300", result, NUM_ZERO);
    fclose(fptr);
    remove(ptr);
}

int main(int argc, char *argv[])
{
    __fwriting_0100();
    __fwriting_0200();
    __fwriting_0300();
    return t_status;
}
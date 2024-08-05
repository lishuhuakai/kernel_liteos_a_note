/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

const int32_t ERRORLEN = -1;

/**
 * @tc.name:      fprintf_0100
 * @tc.desc:      Verify fprintf and stdout process success.
 * @tc.level:     level 0.
 */
void fprintf_0100(void)
{
    char str[] = "This is a fprintf_0100";
    int32_t len = fprintf(stdout, "%s", str);
    EXPECT_EQ("fprintf_0100", len, strlen(str));
}

/**
 * @tc.name:      fprintf_0200
 * @tc.desc:      Verify fprintf and stderr process success.
 * @tc.level:     level 0.
 */
void fprintf_0200(void)
{
    char str[] = "This is a test fprintf_0200";
    int32_t len = fprintf(stderr, "%s", str);
    EXPECT_EQ("fprintf_0200", len, strlen(str));
}

/**
 * @tc.name:      fprintf_0300
 * @tc.desc:      Verify fprintf and fopen("w") process success.
 * @tc.level:     level 0.
 */
void fprintf_0300(void)
{
    char str[] = "这是一个测试";
    int len = 0;
    FILE *fptr = fopen("tempory_testfprintf.txt", "w");
    if (fptr != NULL) {
        len = fprintf(fptr, "%s", str);
        fclose(fptr);
    }
    EXPECT_EQ("fprintf_0300", len, strlen(str));
}

/**
 * @tc.name:      fprintf_0400
 * @tc.desc:      Verify fflush and fopen("r") and fclose and return -1.
 * @tc.level:     level 2.
 */
void fprintf_0400(void)
{
    char str[] = "这是一个测试";
    int32_t len = 0;
    FILE *fptr = fopen("tempory_testfprintf.txt", "r");
    if (fptr != NULL) {
        len = fprintf(fptr, "%s", str);
        fclose(fptr);
    }
    EXPECT_EQ("fprintf_0400", len, ERRORLEN);
    remove("tempory_testfprintf.txt");
}

int main(void)
{
    fprintf_0100();
    fprintf_0200();
    fprintf_0300();
    fprintf_0400();
    return t_status;
}

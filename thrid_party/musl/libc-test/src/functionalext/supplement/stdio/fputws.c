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

#include <wchar.h>
#include <locale.h>
#include "functionalext.h"

const int FAILED = -1;

/**
 * @tc.name      : fputws_0100
 * @tc.desc      : Each parameter is valid and can output a wide character string to a file stream.
 * @tc.level     : Level 0
 */
void fputws_0100(void)
{
    char str[100] = {0};
    const char *ptr = "fputwstest.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_TRUE("fputws_0100", fptr != NULL);
    setlocale(LC_ALL, "en_US.utf8");
    int ret = fputws(L"this is the test", fptr);
    EXPECT_TRUE("fputws_0100", ret >= 0);
    fseek(fptr, 0, SEEK_SET);
    int rsize = fread(str, sizeof(char), 100, fptr);
    EXPECT_EQ("fputws_0100", rsize, ret);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : fputws_0200
 * @tc.desc      : The f parameter is invalid, a wide string cannot be output to a file stream.
 * @tc.level     : Level 2
 */
void fputws_0200(void)
{
    const char *ptr = "fputwstest.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_TRUE("fputws_0200", fptr != NULL);
    fclose(fptr);
    fptr = fopen(ptr, "r");
    EXPECT_TRUE("fputws_0200", fptr != NULL);
    int ret = fputws(L"this is the test", fptr);
    EXPECT_EQ("fputws_0200", ret, FAILED);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

int main(int argc, char *argv[])
{
    fputws_0100();
    fputws_0200();
    return t_status;
}
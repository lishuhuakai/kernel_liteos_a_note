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

#include "functionalext.h"

const char *path = "/data/fputs.txt";

/**
 * @tc.name       : fputs_0100
 * @tc.desc       : Writes the string s to stream
 * @tc.level      : level 0.
 */
void fputs_0100(void)
{
    FILE *fptr = fopen(path, "w+");
    EXPECT_PTRNE("fputs_0100", fptr, NULL);

    int len = fputs("this is a test string", fptr);
    EXPECT_TRUE("fputs_0100", len >= 0);
    fclose(fptr);
    remove(path);
}

/**
 * @tc.name       : fputs_0200
 * @tc.desc       : An empty string is written in the specified file.
 * @tc.level      : level 0.
 */
void fputs_0200(void)
{
    FILE *fptr = fopen(path, "w+");
    EXPECT_PTRNE("fputs_0200", fptr, NULL);

    int len = fputs("", fptr);
    EXPECT_TRUE("fputs_0200", len >= 0);
    fclose(fptr);
    remove(path);
}

/**
 * @tc.name       : fputs_0300
 * @tc.desc       : Verify that an empty string cannot be written to the specified file.
 * @tc.level      : level 2.
 */
void fputs_0300(void)
{
    FILE *p = fopen(path, "w");
    fclose(p);
    p = fopen(path, "r");
    int len = fputs("this is a test string", p);
    EXPECT_EQ("fputs_0300", len, EOF);
    fclose(p);
    remove(path);
}

int main(int argc, char *argv[])
{
    fputs_0100();
    fputs_0200();
    fputs_0300();
    return t_status;
}

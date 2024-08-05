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
#include <stdint.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

const int32_t NUN_ELEVEN = 11;

/**
 * @tc.name      : fgetpos_0100
 * @tc.desc      : File pointer at the end of a file, the position to which the current file pointer points
 * @tc.level     : Level 0
 */
void fgetpos_0100()
{
    fpos_t pos;
    char buff[] = "hello world";
    const char *path = "/data/test.txt";

    FILE *fp = fopen(path, "w+");
    EXPECT_PTRNE("fgetpos_0100", fp, NULL);

    fputs(buff, fp);
    fseek(fp, 0, SEEK_END);

    int result = fgetpos(fp, &pos);
    EXPECT_EQ("fgetpos_0100", result, 0);

    fclose(fp);
    remove(path);
}

/**
 * @tc.name      : fgetpos_0200
 * @tc.desc      : File pointer at the beginning of a file, the location to which the current file pointer points
 * @tc.level     : Level 0
 */
void fgetpos_0200()
{
    fpos_t pos;
    char buff[] = "hello world";
    const char *path = "/data/test.txt";

    FILE *fptr = fopen(path, "w+");
    EXPECT_PTRNE("fgetpos_0100", fptr, NULL);

    fputs(buff, fptr);
    fseek(fptr, 0, SEEK_SET);
    int result = fgetpos(fptr, &pos);
    EXPECT_EQ("fgetpos_0200", result, 0);

    fclose(fptr);
    remove(path);
}

TEST_FUN G_Fun_Array[] = {
    fgetpos_0100,
    fgetpos_0200,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
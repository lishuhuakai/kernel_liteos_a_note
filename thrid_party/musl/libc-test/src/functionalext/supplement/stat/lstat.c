/**
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include "functionalext.h"
#include "filepath_util.h"

extern int __lstat_time64(const char *__restrict, struct stat *__restrict);

typedef void (*TEST_FUN)();

/*
 * @tc.name      : lstat_0100
 * @tc.desc      : Verify that the parameters are valid, get the file status successfully
 * @tc.level     : Level 0
 */
void lstat_0100(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_STAT_TEST_TXT, ptr);

    const char str[] = "this is a sample!";
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    int32_t back = lstat(ptr, &statbuff);
    EXPECT_EQ("lstat_0100", back, 0);
    EXPECT_EQ("lstat_0100", statbuff.st_size, 17);
    fclose(fptr);
    remove(ptr);
}

/*
 * @tc.name      : lstat_time64_0100
 * @tc.desc      : Verify that the parameters are valid, get the file status successfully
 * @tc.level     : Level 0
 */
void lstat_time64_0100(void)
{
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_STAT_TEST_TXT, ptr);

    const char str[] = "this is a sample!";
    FILE *fptr = fopen(ptr, "w+");
    struct stat statbuff;
    fwrite(str, sizeof(char), strlen(str), fptr);
    fseek(fptr, 0L, SEEK_SET);
    int32_t back = __lstat_time64(ptr, &statbuff);
    EXPECT_EQ("lstat_0100", back, 0);
    EXPECT_EQ("lstat_0100", statbuff.st_size, 17);
    fclose(fptr);
    remove(ptr);
}

/*
 * @tc.name      : lstat_0200
 * @tc.desc      : Validation failed to get file status (parameter invalid file does not exist)
 * @tc.level     : Level 2
 */
void lstat_0200(void)
{
    struct stat statbuff;
    int32_t back = lstat(STR_TEST_TXT, &statbuff);
    EXPECT_EQ("lstat_0200", back, -1);
}

/*
 * @tc.name      : lstat_0300
 * @tc.desc      : Verify that the file pathname is valid and the file is a linked file
 * @tc.level     : Level 1
 */
void lstat_0300(void)
{
    struct stat buf[3];
    char ptr[PATH_MAX] = {0};
    char ptrlink[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("tests.txt", ptr);
    FILE_ABSOLUTE_PATH("tests.txt.soft", ptrlink);

    FILE *fptr = fopen(ptr, "w+");
    char cmd[PATH_MAX] = {0};
    snprintf(cmd, sizeof(cmd), "ln -s %s %s", ptr, ptrlink);
    system(cmd);
    struct stat statbuff;
    int32_t back = lstat(ptrlink, &statbuff);
    EXPECT_EQ("lstat_0300", back, 0);
    bool successflag = false;
    if (S_ISLNK(statbuff.st_mode)) {
        successflag = true;
    } else {
        successflag = false;
    }
    EXPECT_TRUE("lstat_0300", successflag);
    fclose(fptr);
    remove(ptr);
    remove(ptrlink);
}

TEST_FUN G_Fun_Array[] = {
    lstat_0100,
    lstat_0200,
    lstat_0300,
    lstat_time64_0100,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}

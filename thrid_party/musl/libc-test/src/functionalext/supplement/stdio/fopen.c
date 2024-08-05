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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

const int32_t INIT_LEN = 0;
const int32_t INCREASE_LEN = 1;
const int32_t CREAT_MODE = 666;

/**
 * @tc.name       : fopen_0100
 * @tc.desc       : Verify fopen("r") and fclose fread and fwrite, fopen success, fread success, fwrite failed
 * @tc.level      : level 0.
 */
void fopen_0100(void)
{
    char abc[100] = {0};
    const char *add = "this is tempory test!";
    const char *wrstring = "to write";
    const char *wstring = "this is tempory test!";
    int ret = creat("tempory_test.txt", CREAT_MODE);
    if (ret < 0) {
        EXPECT_MT("fopen_0100", ret, 0);
        return;
    }
    FILE *fptr = fopen("tempory_test.txt", "w");
    if (fptr != NULL) {
        size_t wrsize = fwrite(wstring, sizeof(char), strlen(wstring), fptr);
        if (wrsize == INIT_LEN) {
            EXPECT_EQ("fopen_0100", wrsize, INIT_LEN);
        }
        fclose(fptr);
    }
    fptr = fopen("tempory_test.txt", "r");
    bool writeFailedFlag = false;
    bool consistentFlag = false;
    if (fptr != NULL) {
        while (!feof(fptr)) {
            int32_t rsize = fread(abc, sizeof(abc), 1, fptr);
            EXPECT_EQ("pread_0100", rsize, 0);
        }
        if (!strncmp(abc, add, strlen(add))) {
            consistentFlag = true;
        }

        size_t wrsize = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
        if (wrsize == INIT_LEN) {
            writeFailedFlag = true;
        }
        fclose(fptr);
    }

    EXPECT_TRUE("fopen_0100", writeFailedFlag);
    EXPECT_TRUE("fopen_0100", consistentFlag);
    remove("tempory_test.txt");
}

/**
 * @tc.name       : fopen_0200
 * @tc.desc       : Verify fopen("r") and fclose and fopen failed
 * @tc.level      : level 2.
 */
void fopen_0200(void)
{
    FILE *fptr = fopen("tempory_test1.txt", "r");
    EXPECT_EQ("fopen_0200", fptr, NULL);
}

/**
 * @tc.name       : fopen_0300
 * @tc.desc       : Verify fopen("w") and fclose and fread and fwrite and fopen success, fread failed, fwrite success
 * @tc.level      : level 0.
 */
void fopen_0300(void)
{
    int32_t rsize = 0;
    char abc[100] = {0};
    const char *wrstring = "to write";
    FILE *fptr = fopen("tempory_test3.txt", "w");
    bool writeSuccessFlag = false;

    if (fptr != NULL) {
        rsize = fread(abc, sizeof(abc), 1, fptr);
        size_t wrsize = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
        if (wrsize == INIT_LEN) {
            EXPECT_EQ("fopen_0300", wrsize, INIT_LEN);
            ;
        } else if (wrsize == strlen(wrstring)) {
            writeSuccessFlag = true;
        }
        fclose(fptr);
    }

    EXPECT_EQ("fopen_0300", rsize, INIT_LEN);
    EXPECT_TRUE("fopen_0300", writeSuccessFlag);
    remove("tempory_test3.txt");
}

/**
 * @tc.name       : fopen_0400
 * @tc.desc       : Verify fopen("w") and fclose and fread and fwrite and fopen success, fread failed, fwrite success
 * @tc.level      : level 1.
 */
void fopen_0400(void)
{
    int32_t rsize = 0;
    char abc[100] = {0};
    const char *wrstring = "to write";
    FILE *fptr = fopen("tempory_test2.txt", "w");
    bool writeSuccessFlag = false;

    if (fptr != NULL) {
        rsize = fread(abc, sizeof(abc), 1, fptr);

        size_t wrsize = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
        if (wrsize == INIT_LEN) {
            EXPECT_EQ("fopen_0400", wrsize, INIT_LEN);
        } else if (wrsize == strlen(wrstring)) {
            writeSuccessFlag = true;
        }
        fclose(fptr);
    }

    EXPECT_EQ("fopen_0400", rsize, INIT_LEN);
    EXPECT_TRUE("fopen_0400", writeSuccessFlag);
    remove("tempory_test2.txt");
}

/**
 * @tc.name       : fopen_0500
 * @tc.desc       : Verify fopen("a") and fclose and fread and fwrite and fopen success, fread success, fwrite success
 * @tc.level      : level 0.
 */
void fopen_0500(void)
{
    char abc[100] = {0};
    const char *add = "this is tempory test!to write";
    const char *wrstring = "to write";
    const char *wstring = "this is tempory test!";
    int ret = creat("tempory_test5.txt", CREAT_MODE);
    if (ret < 0) {
        EXPECT_MT("fopen_0500", ret, 0);
        return;
    }
    FILE *fptr = fopen("tempory_test5.txt", "w");
    if (fptr != NULL) {
        size_t wrsize = fwrite(wstring, sizeof(char), strlen(wstring), fptr);
        if (wrsize == INIT_LEN) {
            EXPECT_EQ("fopen_0500", wrsize, INIT_LEN);
        }
        fclose(fptr);
    }

    fptr = fopen("tempory_test5.txt", "a");
    bool writeSuccessFlag = false;
    bool consistentFlag = false;
    if (fptr != NULL) {
        fseek(fptr, 0, SEEK_SET);
        size_t wrsize = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
        if (wrsize == INIT_LEN) {
            EXPECT_EQ("fopen_0500", wrsize, INIT_LEN);
        } else if (wrsize == strlen(wrstring)) {
            writeSuccessFlag = true;
        }
        fclose(fptr);
    }

    fptr = fopen("tempory_test5.txt", "r");
    if (fptr != NULL) {
        while (!feof(fptr)) {
            int32_t rsize = fread(abc, sizeof(abc), 1, fptr);
            EXPECT_EQ("pread_0500", rsize, 0);
        }
        if (!strncmp(abc, add, strlen(add))) {
            consistentFlag = true;
        }
        fclose(fptr);
    }

    EXPECT_TRUE("fopen_0500", writeSuccessFlag);
    EXPECT_TRUE("fopen_0500", consistentFlag);
    remove("tempory_test5.txt");
}

/**
 * @tc.name       : fopen_0600
 * @tc.desc       : Verify fopen("a") and fclose and fread and fwrite and fopen success, fread success, fwrite success
 * @tc.level      : level 1.
 */
void fopen_0600(void)
{
    int32_t rsize = 0;
    char abc[100] = {0};
    const char *wrstring = "to write";
    FILE *fptr = fopen("tempory_test6.txt", "w");
    bool writeSuccessFlag = false;

    if (fptr != NULL) {
        rsize = fread(abc, sizeof(abc), 1, fptr);

        size_t wrsize = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
        if (wrsize == INIT_LEN) {
            EXPECT_EQ("fopen_0600", wrsize, INIT_LEN);
            ;
        } else if (wrsize == strlen(wrstring)) {
            writeSuccessFlag = true;
        }
        fclose(fptr);
    }

    EXPECT_EQ("fopen_0600", rsize, INIT_LEN);
    EXPECT_TRUE("fopen_0600", writeSuccessFlag);
    remove("tempory_test6.txt");
}

/**
 * @tc.name       : fopen_0700
 * @tc.desc       : Verify fopen("b") and fclose and fread and fwrite and fopen failed
 * @tc.level      : level 2.
 */
void fopen_0700(void)
{
    FILE *fptr = fopen("./tempory_test.txt", "b");
    int32_t flag = INIT_LEN;
    if (fptr == NULL) {
        EXPECT_EQ("fopen_0700", errno, EINVAL);
    } else {
        flag++;
        EXPECT_EQ("fopen_0700", flag, INCREASE_LEN);
    }
}

/**
 * @tc.name       : fopen_0800
 * @tc.desc       : Verify fopen("a") and fclose and fread and fwrite and fopen failed
 * @tc.level      : level 2.
 */
void fopen_0800(void)
{
    FILE *fptr = fopen("./tmp/tempory_test.txt", "a");
    EXPECT_EQ("fopen_0800", fptr, NULL);
}

/**
 * @tc.name       : fopen_0900
 * @tc.desc       : Verify fopen("ae") and fclose and fread and fwrite and fopen success, fread success, fwrite success
 * @tc.level      : level 0.
 */
void fopen_0900(void)
{
    char abc[100] = {0};
    const char *add = "this is tempory test!to write";
    const char *wrstring = "to write";
    const char *wstring = "this is tempory test!";
    int ret = creat("tempory_test9.txt", CREAT_MODE);
    if (ret < 0) {
        EXPECT_MT("fopen_0900", ret, 0);
        return;
    }
    FILE *fptr = fopen("tempory_test9.txt", "w");
    if (fptr != NULL) {
        size_t wrsize = fwrite(wstring, sizeof(char), strlen(wstring), fptr);
        if (wrsize == INIT_LEN) {
            EXPECT_EQ("fopen_0900", wrsize, INIT_LEN);
            ;
        }
        fclose(fptr);
    }

    fptr = fopen("tempory_test9.txt", "ae");
    bool writeSuccessFlag = false;
    bool consistentFlag = false;
    if (fptr != NULL) {
        size_t wrsize = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
        if (wrsize == INIT_LEN) {
            EXPECT_EQ("fopen_0900", wrsize, INIT_LEN);
            ;
        } else if (wrsize == strlen(wrstring)) {
            writeSuccessFlag = true;
        }
        fclose(fptr);
    }

    fptr = fopen("tempory_test9.txt", "r");
    if (fptr != NULL) {
        while (!feof(fptr)) {
            int32_t rsize = fread(abc, sizeof(abc), 1, fptr);
            EXPECT_EQ("pread_0900", rsize, 0);
        }
        if (!strncmp(abc, add, strlen(add))) {
            consistentFlag = true;
        }
        fclose(fptr);
    }
    remove("tempory_test9.txt");

    EXPECT_TRUE("fopen_0900", writeSuccessFlag);
    EXPECT_TRUE("fopen_0900", consistentFlag);
}

int main(void)
{
    fopen_0100();
    fopen_0200();
    fopen_0300();
    fopen_0400();
    fopen_0500();
    fopen_0600();
    fopen_0700();
    fopen_0800();
    fopen_0900();
    return t_status;
}

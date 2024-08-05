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

#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

const int32_t INIT_LEN = 0;

/**
 * @tc.name       : fflush_0100
 * @tc.desc       : Verify fflush process success and return 0.
 * @tc.level      : level 0
 */
void fflush_0100(void)
{
    int32_t ret = fflush(NULL);
    EXPECT_EQ("fflush_0100", ret, INIT_LEN);
}

/**
 * @tc.name       : fflush_0200
 * @tc.desc       : Verify fflush and stdout process success and return 0.
 * @tc.level      : level 0
 */
void fflush_0200(void)
{
    char array[] = "this is fflush test!";
    int32_t ret = fprintf(stdout, "%s", array);
    EXPECT_MT("fflush_0200", ret, 0);
    ret = fflush(NULL);
    EXPECT_EQ("fflush_0200", ret, INIT_LEN);
}

/**
 * @tc.name       : fflush_0300
 * @tc.desc       : Verify fflush and stderr process success and return 0.
 * @tc.level      : level 0
 */
void fflush_0300(void)
{
    char array[] = "this is fflush error!";
    int32_t ret = fprintf(stderr, "%s", array);
    EXPECT_MT("fflush_0300", ret, 0);
    ret = fflush(NULL);
    EXPECT_EQ("fflush_0300", ret, INIT_LEN);
}

/**
 * @tc.name       : fflush_0400
 * @tc.desc       : Verify fflush and fopen("w") and fclose process success and return 0.
 * @tc.level      : level 0
 */
void fflush_0400(void)
{
    FILE *fptr = fopen("/data/tempory_testff.txt", "w");
    int32_t ret = 0;
    if (fptr != NULL) {
        fputs("fopen is success", fptr);
        ret = fflush(fptr);
        fclose(fptr);
    }
    EXPECT_EQ("fflush_0400", ret, INIT_LEN);
}

/**
 * @tc.name       : fflush_0500
 * @tc.desc       : Verify fflush and fopen("r") and fclose process success and return 0.
 * @tc.level      : level 0
 */
void fflush_0500(void)
{
    FILE *fptr = fopen("/data/tempory_testff.txt", "r");
    int32_t ret = 0;
    if (fptr != NULL) {
        ret = fflush(fptr);
        fclose(fptr);
    }
    EXPECT_EQ("fflush_0500", ret, INIT_LEN);
    remove("/data/tempory_testff.txt");
}

int main(void)
{
    fflush_0100();
    fflush_0200();
    fflush_0300();
    fflush_0400();
    fflush_0500();
    return t_status;
}

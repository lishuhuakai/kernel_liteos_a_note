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
#include <string.h>
#include "test.h"

/**
 * @tc.name      : tempnam_0100
 * @tc.desc      : Call the tempnam method to generate a temporary file
 * @tc.level     : Level 0
 */
void tempnam_0100(void)
{
    char *dir = "/data";
    char *pte = "temp";
    char *result = tempnam(dir, pte);
    if (!result) {
        t_error("%s tempnam error cannot create temp file\n", __func__);
    }
    if (!strstr(result, pte)) {
        t_error("%s tempnam get result not contain %s\n", __func__, pte);
    }
}

/**
 * @tc.name      : tempnam_0200
 * @tc.desc      : test tempnam is the result when pfx is empty
 * @tc.level     : Level 1
 */
void tempnam_0200(void)
{
    char *dir = "/data";
    char *pte = NULL;
    char *result = tempnam(dir, pte);
    if (!result) {
        t_error("%s tempnam error cannot create temp file\n", __func__);
    }
}

/**
 * @tc.name      : tempnam_0300
 * @tc.desc      : Test tempnam results when the dir path does not exist
 * @tc.level     : Level 1
 */
void tempnam_0300(void)
{
    char *dir = "/file";
    char *pte = "temp";
    char *result = tempnam(dir, pte);
    if (!result) {
        t_error("%s tempnam error cannot create temp file\n", __func__);
    }
    if (!strstr(result, pte)) {
        t_error("%s tempnam get result not contain %s\n", __func__, pte);
    }
}

/**
 * @tc.name      : tempnam_0400
 * @tc.desc      : Test tempnam when the dir path is a file
 * @tc.level     : Level 1
 */
void tempnam_0400(void)
{
    char *dir = "/file/a.txt";
    char *pte = "temp";
    char *result = tempnam(dir, pte);
    if (!result) {
        t_error("%s tempnam error cannot create temp file\n", __func__);
    }
    if (!strstr(result, pte)) {
        t_error("%s tempnam get result not contain %s\n", __func__, pte);
    }
}

/**
 * @tc.name      : tempnam_0500
 * @tc.desc      : Test tempnam when dir path is empty string
 * @tc.level     : Level 1
 */
void tempnam_0500(void)
{
    char *dir = NULL;
    char *pte = "temp";
    char *result = tempnam(dir, pte);
    if (!result) {
        t_error("%s tempnam error cannot create temp file\n", __func__);
    }
    if (!strstr(result, pte)) {
        t_error("%s tempnam get result not contain %s\n", __func__, pte);
    }
}

int main(int argc, char *argv[])
{
    // tempnam_0100();
    // tempnam_0200();
    // tempnam_0300();
    // tempnam_0400();
    // tempnam_0500();
    return t_status;
}
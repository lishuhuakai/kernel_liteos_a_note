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

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

const char *wrstring = "This is a test sample!";
const char *path = "test.txt";

/**
 * @tc.name      : ftell_0100
 * @tc.desc      : Verify that the file pointer is at the beginning (call the ftell function to see the return value)
 * @tc.level     : Level 0
 */
void ftell_0100(void)
{
    FILE *fptr = fopen(path, "w+");
    if (!fptr) {
        t_error("%s fopen failed\n", __func__);
    }

    size_t ret = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    if (ret < 0) {
        t_error("%s fwrite failed\n", __func__);
    }

    int fret = fseek(fptr, 0L, SEEK_SET);
    if (fret != 0) {
        t_error("%s fseek failed\n", __func__);
    }

    long result = ftell(fptr);
    if (result != 0) {
        t_error("%s ftell failed\n", __func__);
    }

    fclose(fptr);
    remove(path);
}

/**
 * @tc.name      : ftell_0200
 * @tc.desc      : Verify that the file pointer is in the middle (call the ftell function to see the return value)
 * @tc.level     : Level 0
 */
void ftell_0200(void)
{
    FILE *fptr = fopen(path, "w+");
    if (!fptr) {
        t_error("%s fopen failed\n", __func__);
    }

    size_t ret = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    if (ret < 0) {
        t_error("%s fwrite failed\n", __func__);
    }

    int fret = fseek(fptr, 8L, SEEK_SET);
    if (fret != 0) {
        t_error("%s fseek failed\n", __func__);
    }

    long result = ftell(fptr);
    if (result != 8) {
        t_error("%s ftell failed\n", __func__);
    }

    fclose(fptr);
    remove(path);
}

/**
 * @tc.name      : ftell_0300
 * @tc.desc      : Verify that the file pointer is at the end (call the ftell function to see the return value)
 * @tc.level     : Level 0
 */
void ftell_0300(void)
{
    FILE *fptr = fopen(path, "w+");
    if (!fptr) {
        t_error("%s fopen failed\n", __func__);
    }

    size_t ret = fwrite(wrstring, sizeof(char), strlen(wrstring), fptr);
    if (ret < 0) {
        t_error("%s fwrite failed\n", __func__);
    }

    int fret = fseek(fptr, 0L, SEEK_END);
    if (fret != 0) {
        t_error("%s fseek failed\n", __func__);
    }
    long result = ftell(fptr);
    if (result != 22) {
        t_error("%s ftell failed\n", __func__);
    }

    fclose(fptr);
    remove(path);
}

int main(int argc, char *argv[])
{
    ftell_0100();
    ftell_0200();
    ftell_0300();

    return t_status;
}
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
#include "filepath_util.h"

/**
 * @tc.name       : fputc_0100
 * @tc.desc       : Verify that a character can be written to the file
 * @tc.level      : level 0.
 */
void fputc_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FPUTC_TXT, path);
    FILE *fptr = fopen(path, "w");
    int Exit = fputc('a', fptr);
    EXPECT_EQ("fputc_0100", Exit, 97);
    fclose(fptr);
}

/**
 * @tc.name       : fputc_0200
 * @tc.desc       : Verify that a character cannot be written to the file
 * @tc.level      : level 2.
 */
void fputc_0200(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FPUTC_TXT, path);
    FILE *fptr = fopen(path, "r");
    int Exit = fputc('a', fptr);
    EXPECT_EQ("fputc_0200", Exit, EOF);
    fclose(fptr);
    remove(path);
}

int main(void)
{
    fputc_0100();
    fputc_0200();
    return t_status;
}
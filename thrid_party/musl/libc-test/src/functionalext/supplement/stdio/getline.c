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
#include "functionalext.h"
#include "filepath_util.h"

/**
 * @tc.name      : getline_0100
 * @tc.desc      : Verify that a row of data can be read (parameters are valid).
 * @tc.level     : Level 0
 */
void getline_0100(void)
{
    char *wrstring = "helloworld";
    char *line = NULL;
    size_t len = 0;
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_TEST_TXT, ptr);
    FILE *fp = fopen(ptr, "w+");
    EXPECT_PTRNE("getline_0100", fp, NULL);
    fwrite(wrstring, sizeof(char), strlen(wrstring), fp);
    fseek(fp, 0, SEEK_SET);

    ssize_t read = getline(&line, &len, fp);
    EXPECT_EQ("getline_0100", read, 10);
    EXPECT_STREQ("getline_0100", line, "helloworld");

    fclose(fp);
    remove(ptr);
}

/**
 * @tc.name      : getline_0200
 * @tc.desc      : Validation cannot read a row of data (parameter is NULL).
 * @tc.level     : Level 2
 */
void getline_0200(void)
{
    size_t len = 0;
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, ptr);
    FILE *fp = fopen(ptr, "w+");
    EXPECT_PTRNE("getline_0100", fp, NULL);

    ssize_t read = getline(NULL, &len, fp);
    EXPECT_EQ("getline_0200", read, -1);

    fclose(fp);
    remove(ptr);
}

/**
 * @tc.name      : getline_0300
 * @tc.desc      : TValidation cannot read a row of data (parameter is 0).
 * @tc.level     : Level 2
 */
void getline_0300(void)
{
    char *line = NULL;
    char ptr[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH(STR_FILE_TXT, ptr);
    FILE *fp = fopen(ptr, "w+");
    EXPECT_PTRNE("getline_0100", fp, NULL);

    ssize_t read = getline(&line, 0, fp);
    EXPECT_EQ("getline_0300", read, -1);
    fclose(fp);
    remove(ptr);
}

int main(int argc, char *argv[])
{
    getline_0100();
    getline_0200();
    getline_0300();
    return t_status;
}
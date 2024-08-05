/*
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

#include <string.h>
#include "functionalext.h"

/**
 * @tc.name      : getdelim_0100
 * @tc.desc      : Verify that the file content can be read with the specified delimiter (parameters are valid)
 * @tc.level     : Level 0
 */
void getdelim_0100(void)
{
    char *wrstring = "hello,world";
    char *line = NULL;
    size_t len = 0;

    FILE *fp = fopen("getdelim.txt", "w+");
    EXPECT_PTRNE("getdelim_0100", fp, NULL);

    fwrite(wrstring, sizeof(char), strlen(wrstring), fp);
    fseek(fp, 0, SEEK_SET);

    ssize_t result = getdelim(&line, &len, ',', fp);
    EXPECT_TRUE("getdelim_0100", result > 0);
    EXPECT_TRUE("getdelim_0100", strcmp(line, "hello,") == 0);
    EXPECT_TRUE("getdelim_0100", len > strlen("hello,"));

    remove("getdelim.txt");
}

/**
 * @tc.name      : getdelim_0200
 * @tc.desc      : Verify that the delimiter cannot be specified to read the file content (the s parameter is invalid)
 * @tc.level     : Level 2
 */
void getdelim_0200(void)
{
    char *line = NULL;
    char *wrstring = "helloworld";

    FILE *fp = fopen("getdelim.txt", "w+");
    EXPECT_PTRNE("getdelim_0200", fp, NULL);

    ssize_t result = getdelim(&line, NULL, 'l', fp);
    EXPECT_EQ("getdelim_0200", result, -1);
    remove("getdelim.txt");
}

/**
 * @tc.name      : getdelim_0300
 * @tc.desc      : Verify that the delimiter cannot be specified to read the file content (the n parameter is invalid)
 * @tc.level     : Level 2
 */
void getdelim_0300(void)
{
    size_t a = 0;
    char *wrstring = "helloworld";

    FILE *fp = fopen("getdelim.txt", "w+");
    EXPECT_PTRNE("getdelim_0300", fp, NULL);

    ssize_t result = getdelim(NULL, &a, 'l', fp);
    EXPECT_EQ("getdelim_0300", result, -1);
    remove("getdelim.txt");
}

int main(int argc, char *argv[])
{
    getdelim_0100();
    getdelim_0200();
    getdelim_0300();

    return t_status;
}
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

#include <stdio_ext.h>
#include "functionalext.h"

const char *path = "/data/fpending.txt";

/**
 * @tc.name      : __fpending_0100
 * @tc.desc      : The parameter is valid, f is the file opened in w mode, there is data in the buffer,
 *                 get the number of bytes in the current buffer.
 * @tc.level     : Level 0
 */
void __fpending_0100(void)
{
    char *str = "This is a test";
    FILE *fp = fopen(path, "w");
    EXPECT_PTRNE("__fpending_0100", fp, NULL);

    fputs(str, fp);
    size_t ret = __fpending(fp);
    EXPECT_TRUE("__fpending_0100", ret > 0);
    EXPECT_EQ("__fpending_0100", ret, strlen(str));

    fclose(fp);
    remove(path);
}

/**
 * @tc.name      : __fpending_0200
 * @tc.desc      : The parameter is valid, f is the file opened in w mode, there is no data in the buffer,
 *                 et the number of bytes in the current buffer.
 * @tc.level     : Level 1
 */
void __fpending_0200(void)
{
    char *str = "This is a test";
    FILE *fp = fopen(path, "w");
    EXPECT_PTRNE("__fpending_0200", fp, NULL);

    fputs(str, fp);
    fflush(fp);
    size_t ret = __fpending(fp);
    EXPECT_EQ("__fpending_0200", ret, 0);

    fclose(fp);
    remove(path);
}

/**
 * @tc.name      : __fpending_0300
 * @tc.desc      : The parameter is valid, f is the file opened in r mode, and the number of bytes in
 *                 the current buffer cannot be obtained.
 * @tc.level     : Level 2
 */
void __fpending_0300(void)
{
    char *str = "This is a test";
    FILE *fp = fopen(path, "w");
    EXPECT_PTRNE("__fpending_0300", fp, NULL);

    fputs(str, fp);
    fclose(fp);

    FILE *ffp = fopen(path, "r");
    size_t ret = __fpending(ffp);
    EXPECT_EQ("__fpending_0300", ret, 0);

    fclose(ffp);
    remove(path);
}

int main(int argc, char *argv[])
{
    __fpending_0100();
    __fpending_0200();
    __fpending_0300();
    return t_status;
}
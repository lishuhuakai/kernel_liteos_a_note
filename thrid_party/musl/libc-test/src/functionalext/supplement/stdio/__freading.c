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

const char *path = "/data/freading.txt";

/**
 * @tc.name      : __freading_0100
 * @tc.desc      : The parameter is valid, the file is opened in read-only mode, and the file stream can
 *                 be judged to be read-only.
 * @tc.level     : Level 0
 */
void __freading_0100(void)
{
    char *str = "This is a test";
    FILE *fp = fopen(path, "w");
    fputs(str, fp);
    fclose(fp);
    FILE *ffp = fopen(path, "r");
    size_t ret = __freading(ffp);
    EXPECT_NE("__freading_0100", ret, 0);
    fclose(ffp);
    remove(path);
}

/**
 * @tc.name      : __freading_0200
 * @tc.desc      : The parameter is valid, the file has just been read, and it is judged that the file
 *                 stream has been read.
 * @tc.level     : Level 0
 */
void __freading_0200(void)
{
    char *str = "This is a test";
    FILE *fp = fopen(path, "w");
    fputs(str, fp);
    fclose(fp);
    FILE *ffp = fopen(path, "r");
    char buf[100];
    fgets(buf, 100, ffp);
    size_t ret = __freading(ffp);
    EXPECT_NE("__freading_0200", ret, 0);
    fclose(ffp);
    remove(path);
}

/**
 * @tc.name      : __freading_0300
 * @tc.desc      : The parameter is valid, the file has just been written, and it is judged that the file stream
 *                 has not been read.
 * @tc.level     : Level 2
 */
void __freading_0300(void)
{
    char *str = "This is a test";
    FILE *fp = fopen(path, "w");
    fputs(str, fp);
    size_t ret = __freading(fp);
    EXPECT_EQ("__freading_0300", ret, 0);
    fclose(fp);
    remove(path);
}

int main(int argc, char *argv[])
{
    __freading_0100();
    __freading_0200();
    __freading_0300();
    return t_status;
}
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

#include <stdlib.h>
#include <sys/stat.h>
#include "functionalext.h"

/**
 * @tc.name      : readlink_0100
 * @tc.desc      : Symlink to path exists and bufsize is large enough to return the number of characters correctly.
 * @tc.level     : Level 0
 */
void readlink_0100(void)
{
    struct stat sb;
    char *buf;
    int ret;
    char *wstr = "this is a test\n";
    const char *softptr = "./readlink.txt.soft";
    buf = malloc(sb.st_size + 1);
    const char *ptr = "readlink.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_TRUE("readlink_0100", fptr != NULL);
    int size = fwrite(wstr, sizeof(char), strlen(wstr), fptr);
    EXPECT_EQ("readlink_0100", size, strlen(wstr));
    fclose(fptr);
    int link = symlink("./readlink.txt", softptr);
    EXPECT_EQ("readlink_0100", link, 0);
    ret = readlink(softptr, buf, sizeof(buf));
    EXPECT_EQ("readlink_0100", ret, sizeof(buf));
    remove(ptr);
    remove(softptr);
    buf = NULL;
    wstr = NULL;
    softptr = NULL;
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : readlink_0200
 * @tc.desc      : Symlink to path exists and the bufsize is not large enough to
 *                 return the number of characters correctly.
 * @tc.level     : Level 1
 */
void readlink_0200(void)
{
    struct stat sb;
    char *buf;
    int ret;
    char *wstr = "this is a test\n";
    const char *softptr = "./readlink.txt.soft";
    buf = malloc(sb.st_size + 1);
    const char *ptr = "readlink.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_TRUE("readlink_0200", fptr != NULL);
    int size = fwrite(wstr, sizeof(char), strlen(wstr), fptr);
    EXPECT_EQ("readlink_0200", size, strlen(wstr));
    fclose(fptr);
    int link = symlink("./readlink.txt", softptr);
    EXPECT_EQ("readlink_0200", link, 0);
    ret = readlink(softptr, buf, sizeof(buf) - 2);
    EXPECT_EQ("readlink_0200", ret, sizeof(buf) - 2);
    remove(ptr);
    remove(softptr);
    buf = NULL;
    wstr = NULL;
    softptr = NULL;
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : readlink_0300
 * @tc.desc      : Symbolic link that does not exist in path cannot have the correct number of characters.
 * @tc.level     : Level 2
 */
void readlink_0300(void)
{
    struct stat sb;
    char *buf;
    int ret;
    const char *softptr = "./noreadlink.txt.soft";
    buf = malloc(sb.st_size + 1);
    ret = readlink(softptr, buf, sizeof(buf));
    EXPECT_TRUE("readlink_0300", ret < 0);
    buf = NULL;
    softptr = NULL;
}

int main(int argc, char *argv[])
{
    readlink_0100();
    readlink_0200();
    readlink_0300();

    return t_status;
}
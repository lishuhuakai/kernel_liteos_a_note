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
#include <sys/stat.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : chown_0100
 * @tc.desc      : Verify the owner user and group of the changed file
 * @tc.level     : Level 0
 */
void chown_0100(void)
{
    struct stat buf;
    const char *ptr = "/data/test.txt";
    FILE *fp = fopen(ptr, "w+");
    EXPECT_PTRNE("chown_0100", fp, NULL);

    int result = chown(ptr, 0, 0);
    int ret = stat(ptr, &buf);
    EXPECT_EQ("chown_0100", ret, 0);
    EXPECT_EQ("chown_0100", result, 0);
    EXPECT_EQ("chown_0100", buf.st_uid, 0);
    EXPECT_EQ("chown_0100", buf.st_gid, 0);

    fclose(fp);
    remove(ptr);
}

/**
 * @tc.name      : chown_0200
 * @tc.desc      : Verify the owner user of the changed file
 * @tc.level     : Level 0
 */
void chown_0200(void)
{
    struct stat buf;
    const char *ptr = "/data/test.txt";
    FILE *fp = fopen(ptr, "w+");
    EXPECT_PTRNE("chown_0200", fp, NULL);

    int result = chown(ptr, 0, -1);
    int ret = stat(ptr, &buf);
    EXPECT_EQ("chown_0200", ret, 0);
    EXPECT_EQ("chown_0200", result, 0);
    EXPECT_EQ("chown_0200", buf.st_uid, 0);
    printf("0200: %d\n", buf.st_gid);
    fclose(fp);
    remove(ptr);
}

/**
 * @tc.name      : chown_0300
 * @tc.desc      : Verify the group that changed the owner of the file
 * @tc.level     : Level 0
 */
void chown_0300(void)
{
    struct stat buf;
    const char *ptr = "/data/test.txt";
    FILE *fp = fopen(ptr, "w+");
    EXPECT_PTRNE("chown_0300", fp, NULL);

    int result = chown(ptr, -1, 0);
    int ret = stat(ptr, &buf);
    EXPECT_EQ("chown_0300", ret, 0);
    EXPECT_EQ("chown_0300", result, 0);
    EXPECT_EQ("chown_0300", buf.st_gid, 0);
    printf("0300: %d\n", buf.st_uid);

    fclose(fp);
    remove(ptr);
}

/**
 * @tc.name      : chown_0400
 * @tc.desc      : Verify that the user and group cannot change the owner of the file (parameter path is invalid)
 * @tc.level     : Level 2
 */
void chown_0400(void)
{
    int result = chown("/data/test.txt", 0, 0);
    EXPECT_EQ("chown_0400", result, -1);
}

/**
 * @tc.name      : chown_0500
 * @tc.desc      : Verify user and group that cannot change the owner of the file (parameter path is NULL)
 * @tc.level     : Level 2
 */
void chown_0500(void)
{
    int result = chown(NULL, 0, 0);
    EXPECT_EQ("chown_0500", result, -1);
}

int main(int argc, char *argv[])
{
    chown_0100();
    chown_0200();
    chown_0300();
    chown_0400();
    chown_0500();
    return t_status;
}
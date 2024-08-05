/**
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

#include <fcntl.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : mkostemp_0100
 * @tc.desc      : Verify mkostemp process success. Provide the correct template and no fixed suffix,
 *                 specified in flags: O_APPEND, create a temporary file.
 * @tc.level     : Level 0
 */
void mkostemp_0100(void)
{
    char tmpfile[] = "/data/mkostemp_0100_XXXXXX";
    int fd = mkostemp(tmpfile, O_APPEND);
    EXPECT_TRUE("mkostemp_0100", fd != -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemp_0100", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkostemp_0200
 * @tc.desc      : Verify mkostemp process success. Provide the correct template and no fixed suffix,
 *                 specified in flags: O_CLOEXEC, create a temporary file.
 * @tc.level     : Level 0
 */
void mkostemp_0200(void)
{
    char tmpfile[] = "/data/mkostemp_0200_XXXXXX";
    int fd = mkostemp(tmpfile, O_CLOEXEC);
    EXPECT_TRUE("mkostemp_0200", fd != -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemp_0200", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkostemp_0300
 * @tc.desc      : Verify mkostemp process success. Provide the correct template and no fixed suffix,
 *                 specified in flags: O_CLOEXEC, create a temporary file.
 * @tc.level     : Level 0
 */
void mkostemp_0300(void)
{
    char tmpfile[] = "/data/mkostemp_0300_XXXXXX";
    int fd = mkostemp(tmpfile, O_SYNC);
    EXPECT_TRUE("mkostemp_0300", fd != -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemp_0300", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkostemp_0400
 * @tc.desc      : Verify mkostemp process fail. Provide error template, specified in flags: O_APPEND,
 *                 create temp file fail.
 * @tc.level     : Level 2
 */
void mkostemp_0400(void)
{
    char tmpfile[] = "/tmp/mkostemp_0400.dat";
    int fd = mkostemp(tmpfile, O_APPEND);
    EXPECT_TRUE("mkostemp_0400", fd == -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemp_0400", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkostemp_0500
 * @tc.desc      : Verify mkostemp process fail. Provide error template, specified in flags: O_CLOEXEC,
 *                 create temp file fail.
 * @tc.level     : Level 2
 */
void mkostemp_0500(void)
{
    char tmpfile[] = "/tmp/mkostemp_0500.dat";
    int fd = mkostemp(tmpfile, O_CLOEXEC);
    EXPECT_TRUE("mkostemp_0500", fd == -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemp_0500", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkostemp_0600
 * @tc.desc      : Verify mkostemp process fail. Provide error template, specified in flags: O_SYNC,
 *                 create temp file fail.
 * @tc.level     : Level 2
 */
void mkostemp_0600(void)
{
    char tmpfile[] = "/tmp/mkostemp_0600.dat";
    int fd = mkostemp(tmpfile, O_SYNC);
    EXPECT_TRUE("mkostemp_0600", fd == -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkostemp_0600", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

int main(void)
{
    mkostemp_0100();
    mkostemp_0200();
    mkostemp_0300();
    mkostemp_0400();
    mkostemp_0500();
    mkostemp_0600();
    return t_status;
}
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

#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : mkstemp_0100
 * @tc.desc      : Verify mkstemp process success. Provide the correct template and no fixed suffix, create a
 *                 temporary file
 * @tc.level     : Level 0
 */
void mkstemp_0100(void)
{
    char tmpfile[] = "/data/mkstemp_0100_XXXXXX";
    int fd = mkstemp(tmpfile);
    EXPECT_TRUE("mkstemp_0100", fd != -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkstemp_0100", cnt == strlen(tmpfile));
        close(fd);
        char rmfile[128];
        int len = sprintf(rmfile, "rm %s", tmpfile);
        if (len > 0) {
            system(rmfile);
        }
    }
}

/**
 * @tc.name      : mkstemp_0200
 * @tc.desc      : Verify mkstemp process fail. Provide error template, create temp file fail
 * @tc.level     : Level 2
 */
void mkstemp_0200(void)
{
    char tmpfile[] = "/data/mkstemp_0200.dat";
    int fd = mkstemp(tmpfile);
    EXPECT_TRUE("mkstemp_0200", fd == -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("mkstemp_0200", cnt == strlen(tmpfile));
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
    mkstemp_0100();
    mkstemp_0200();
    return t_status;
}
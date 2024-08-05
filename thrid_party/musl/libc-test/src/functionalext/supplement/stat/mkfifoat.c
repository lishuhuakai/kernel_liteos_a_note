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

#include <errno.h>
#include <sys/stat.h>
#include "functionalext.h"

#define TEST_FIFO_MODE 0666
#define TEST_AT_FDCWD -100

/**
 * @tc.name      : mkfifoat_0100
 * @tc.desc      : Verify mkfifoat process success. Create named pipe with correct parameters.
 *                 The pathname given in pathname is absolute.
 * @tc.level     : Level 0
 */
void mkfifoat_0100(void)
{
    struct stat sb;
    const char *path = "/data/mkfifoat_0100";
    int ret = mkfifoat(-1, path, S_IFIFO | TEST_FIFO_MODE);
    EXPECT_EQ("mkfifoat_0100", ret, 0);
    ret = stat(path, &sb);
    EXPECT_EQ("mkfifoat_0100", ret, 0);
    EXPECT_TRUE("mkfifoat_0100", S_ISFIFO(sb.st_mode));
    unlink(path);
}

/**
 * @tc.name      : mkfifoat_0200
 * @tc.desc      : Verify mkfifoat process success. Create named pipe with correct parameters.
 *                 The pathname given in pathname is relative.
 * @tc.level     : Level 0
 */
void mkfifoat_0200(void)
{
    struct stat sb;
    const char *path = "mkfifoat_0200";
    int ret = mkfifoat(TEST_AT_FDCWD, path, S_IFIFO | TEST_FIFO_MODE);
    EXPECT_EQ("mkfifoat_0200", ret, 0);
    ret = stat(path, &sb);
    EXPECT_EQ("mkfifoat_0200", ret, 0);
    EXPECT_TRUE("mkfifoat_0200", S_ISFIFO(sb.st_mode));
    unlink(path);
}

/**
 * @tc.name      : mkfifoat_0300
 * @tc.desc      : Verify mkfifoat process fail. Create named pipe with exception argument.
 * @tc.level     : Level 2
 */
void mkfifoat_0300(void)
{
    int ret = mkfifoat(-1, "mkfifoat_0300", S_IFIFO | TEST_FIFO_MODE);
    EXPECT_EQ("mkfifoat_0300", ret, -1);
}

int main(void)
{
    mkfifoat_0100();
    mkfifoat_0200();
    mkfifoat_0300();
    return t_status;
}
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

/**
 * @tc.name      : mknodat_0100
 * @tc.desc      : Verify mknodat process success. Create named pipe with correct parameters.
 *                 The pathname given in pathname is absolute.
 * @tc.level     : Level 0
 */
void mknodat_0100(void)
{
    struct stat sb;
    const char *path = "/data/mknodat_0100";
    int ret = mknodat(-1, path, S_IFIFO | TEST_FIFO_MODE, 0);
    EXPECT_EQ("mknodat_0100", ret, 0);
    ret = stat(path, &sb);
    EXPECT_EQ("mknodat_0100", ret, 0);
    EXPECT_TRUE("mknodat_0100", S_ISFIFO(sb.st_mode));
    unlink(path);
}

/**
 * @tc.name      : mknodat_0200
 * @tc.desc      : Verify mknodat process fail. Because pathname already exists.
 * @tc.level     : Level 0
 */
void mknodat_0200(void)
{
    struct stat sb;
    const char *path = "/data/mknodat_0200";
    int ret = mknodat(-1, path, S_IFIFO | TEST_FIFO_MODE, 0);
    EXPECT_EQ("mknodat_0200", ret, 0);
    ret = stat(path, &sb);
    EXPECT_EQ("mknodat_0200", ret, 0);
    EXPECT_TRUE("mknodat_0200", S_ISFIFO(sb.st_mode));
    ret = mknodat(-1, path, S_IFIFO | TEST_FIFO_MODE, 0);
    EXPECT_EQ("mknodat_0200", ret, -1);
    unlink(path);
}

int main(void)
{
    mknodat_0100();
    mknodat_0200();
    return t_status;
}
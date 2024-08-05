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
 * @tc.name      : mkfifo_0100
 * @tc.desc      : Create named pipe with correct parameters
 * @tc.level     : Level 0
 */
void mkfifo_0100(void)
{
    struct stat sb;
    const char *path = "/data/test_fifo";
    int ret = mkfifo(path, S_IFIFO | TEST_FIFO_MODE);
    EXPECT_EQ("mkfifo_0100", ret, 0);
    ret = stat(path, &sb);
    EXPECT_EQ("mkfifo_0100", ret, 0);
    EXPECT_TRUE("mkfifo_0100", S_ISFIFO(sb.st_mode));
    unlink(path);
}

/**
 * @tc.name      : mkfifo_0200
 * @tc.desc      : Create named pipe with exception argument
 * @tc.level     : Level 0
 */
void mkfifo_0200(void)
{
    int ret = mkfifo("/", TEST_FIFO_MODE);
    EXPECT_EQ("mkfifo_0200", ret, -1);
    EXPECT_EQ("mkfifo_0200", errno, EEXIST);
}

int main(void)
{
    mkfifo_0100();
    mkfifo_0200();
    return t_status;
}
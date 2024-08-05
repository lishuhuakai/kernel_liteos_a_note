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
#include "functionalext.h"

/**
 * @tc.name      : dprintf_0100
 * @tc.desc      : Verify the number of characters returned by the function (if the file parameter is stdout)
 * @tc.level     : Level 0
 */
void dprintf_0100(void)
{
    int result = dprintf(1, "This is a test!!QAZ@WSX12");
    printf("\n");
    int ret = strlen("This is a test!!QAZ@WSX12");
    EXPECT_EQ("dprintf_0100", result, ret);
}

/**
 * @tc.name      : dprintf_0200
 * @tc.desc      : Verify the number of characters returned by the function (if the file argument is stderr)
 * @tc.level     : Level 0
 */
void dprintf_0200(void)
{
    int result = dprintf(2, "This is a test!");
    printf("\n");
    int ret = strlen("This is a test!");
    EXPECT_EQ("dprintf_0200", result, ret);
}

/**
 * @tc.name      : dprintf_0300
 * @tc.desc      : Verify the number of characters returned by the function
                  (if the file is successfully opened in W mode and its return value is used as a file parameter)
 * @tc.level     : Level 0
 */
void dprintf_0300(void)
{
    const char *path = "/data/test.txt";
    int fd = open(path, O_CREAT | O_WRONLY, TEST_MODE);
    EXPECT_NE("dprintf_0300", fd, -1);

    int result = dprintf(fd, "This is a test!");
    int ret = strlen("This is a test!");

    EXPECT_EQ("dprintf_0300", result, ret);

    close(fd);
    remove(path);
}

/**
 * @tc.name      : dprintf_0400
 * @tc.desc      : Verify the number of characters returned by the function
                  (if the file is successfully opened in R mode and its return value is used as a file parameter)
 * @tc.level     : Level 2
 */
void dprintf_0400(void)
{
    const char *path = "/data/test.txt";
    int fd = open(path, O_CREAT | O_RDONLY, TEST_MODE);
    EXPECT_NE("dprintf_0400", fd, -1);

    int result = dprintf(fd, "This is a test!");
    EXPECT_EQ("dprintf_0400", result, -1);

    close(fd);
    remove(path);
}

int main(int argc, char *argv[])
{
    dprintf_0100();
    dprintf_0200();
    dprintf_0300();
    dprintf_0400();
    return t_status;
}
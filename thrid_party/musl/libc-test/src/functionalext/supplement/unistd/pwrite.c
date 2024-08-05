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
#include <unistd.h>
#include "functionalext.h"

#define TEST_BUFFER_SIZE 64

void rm_file(const char *name)
{
    if (access(name, F_OK) == 0) {
        unlink(name);
    }
}

/**
 * @tc.name      : pwrite_0100
 * @tc.desc      : Write to a file descriptor at a given offset
 * @tc.level     : Level 0
 */
void pwrite_0100(void)
{
    const char *txt = "This is pwrite_0100 test.";
    char buffer[TEST_BUFFER_SIZE];
    memset(buffer, 0x0, sizeof(buffer));

    int fd = open("pwrite_0100", O_CREAT | O_RDWR, TEST_MODE);
    EXPECT_NE("pwrite_0100", fd, ERREXPECT);
    if (fd == -1) {
        return;
    }
    size_t cnt = pwrite(fd, txt, strlen(txt), 0);
    EXPECT_EQ("pwrite_0100", cnt, strlen(txt));

    lseek(fd, 0, SEEK_SET);
    cnt = pread(fd, buffer, TEST_BUFFER_SIZE, 0);
    EXPECT_EQ("pwrite_0100", cnt, strlen(txt));
    EXPECT_STREQ("pwrite_0100", txt, buffer);
    close(fd);
    rm_file("pwrite_0100");
}

/**
 * @tc.name      : pwrite_0200
 * @tc.desc      : Provide illegal parameter data, write data to the open file
 * @tc.level     : Level 2
 */
void pwrite_0200(void)
{
    const char *txt = "This is pwrite_0200 test.";
    size_t cnt = pwrite(-1, txt, strlen(txt), 0);
    EXPECT_EQ("pwrite_0200", cnt, (size_t)(-1));

    int fd = open("pwrite_0200", O_CREAT | O_RDWR, TEST_MODE);
    EXPECT_NE("pwrite_0200", fd, -1);
    if (fd == -1) {
        return;
    }

    cnt = pwrite(fd, txt, 0, 0);
    EXPECT_EQ("pwrite_0200", cnt, CMPFLAG);

    cnt = pwrite(fd, NULL, 0, 0);
    EXPECT_EQ("pwrite_0200", cnt, CMPFLAG);
    close(fd);
    rm_file("pwrite_0200");
}

int main(void)
{
    pwrite_0100();
    pwrite_0200();
    return t_status;
}

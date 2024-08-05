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
#include <unistd.h>
#include "functionalext.h"

#define TEST_BUFFER_SIZE 64

/**
 * @tc.name      : pread_0100
 * @tc.desc      : Read data from a file into a buffer
 * @tc.level     : Level 1
 */
void pread_0100(void)
{
    const char *txt = "This is pread_0100 test.";
    char buffer[TEST_BUFFER_SIZE];
    memset(buffer, 0x0, sizeof(buffer));
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("pread_0100", fp, NULL);
        return;
    }
    int fd = fileno(fp);
    pwrite(fd, txt, strlen(txt), 0);
    lseek(fd, 0, SEEK_SET);
    size_t cnt = pread(fd, buffer, TEST_BUFFER_SIZE, 0);
    EXPECT_EQ("pread_0100", cnt, strlen(txt));
    EXPECT_STREQ("pread_0100", txt, buffer);
    int ret = fclose(fp);
    EXPECT_EQ("pread_0100", ret, 0);
}

/**
 * @tc.name      : pread_0200
 * @tc.desc      : The current position is at the end, read data from a file into a buffer
 * @tc.level     : Level 1
 */
void pread_0200(void)
{
    const char *txt = "This is pread_0200 test.";
    char buffer[TEST_BUFFER_SIZE];
    memset(buffer, 0x0, sizeof(buffer));
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("pread_0200", fp, NULL);
        return;
    }
    int fd = fileno(fp);
    pwrite(fd, txt, strlen(txt), 0);
    size_t cnt = pread(fd, buffer, TEST_BUFFER_SIZE, 0);
    EXPECT_EQ("pread_0200", cnt, strlen(txt));
    EXPECT_STREQ("pread_0200", txt, buffer);
    int ret = fclose(fp);
    EXPECT_EQ("pread_0200", ret, 0);
}

/**
 * @tc.name      : pread_0300
 * @tc.desc      : The current position is at the end, read the following data
 * @tc.level     : Level 2
 */
void pread_0300(void)
{
    const char *txt = "This is pread_0300 test.";
    char buffer[TEST_BUFFER_SIZE];
    memset(buffer, 0x0, sizeof(buffer));
    FILE *fp = tmpfile();
    if (!fp) {
        EXPECT_PTRNE("pread_0300", fp, NULL);
        return;
    }
    int fd = fileno(fp);
    pwrite(fd, txt, strlen(txt), 0);
    size_t cnt = pread(fd, buffer, TEST_BUFFER_SIZE, strlen(txt));
    EXPECT_EQ("pread_0300", cnt, 0);
    int ret = fclose(fp);
    EXPECT_EQ("pread_0300", ret, 0);
}

int main(void)
{
    pread_0100();
    pread_0200();
    pread_0300();
    return t_status;
}
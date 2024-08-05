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

const int SIZE = 20;

/**
 * @tc.name      : read_0100
 * @tc.desc      : The parameters are valid and the actual number of characters can be read.
 * @tc.level     : Level 0
 */
void read_0100(void)
{
    char str[] = "this is a readtest\n";
    char buffer[1024] = {0};
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    EXPECT_TRUE("read_0100", fd >= 0);
    int retwrite = write(fd, str, sizeof(str));
    EXPECT_EQ("read_0100", retwrite, SIZE);
    lseek(fd, 0, SEEK_SET);
    int size = read(fd, buffer, 1024);
    EXPECT_EQ("read_0100", size, SIZE);
    close(fd);
    remove("/data/readtest.txt");
}

int main(int argc, char *argv[])
{
    read_0100();
    return t_status;
}
/*
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
#include <sys/stat.h>
#include "functionalext.h"

/**
 * @tc.name      : creat_0100
 * @tc.desc      : Each parameter is valid and a new file can be created.
 * @tc.level     : Level 0
 */
void creat_0100(void)
{
    int fd = -1;
    const char *ptr = "/data/creattest.txt";
    fd = creat(ptr, O_RDWR | O_CREAT);
    EXPECT_NE("creat_0100", fd, -1);
    close(fd);
    remove(ptr);
    ptr = NULL;
}

/**
 * @tc.name      : creat_0200
 * @tc.desc      : The filename parameter is invalid, a new file cannot be created.
 * @tc.level     : Level 2
 */
void creat_0200(void)
{
    int fd = 0;
    const char *ptr = "";
    fd = open(ptr, O_RDONLY);
    EXPECT_EQ("creat_0200", fd, -1);
    close(fd);
    remove(ptr);
    ptr = NULL;
}

/**
 * @tc.name      : creat_0300
 * @tc.desc      : The mode parameter is invalid and a new file cannot be created.
 * @tc.level     : Level 0
 */
void creat_0300(void)
{
    int fd = 0;
    const char *ptr = "/data/creattest.txt";
    fd = open(ptr, O_WRONLY);
    EXPECT_EQ("creat_0300", fd, -1);
    close(fd);
    remove(ptr);
    ptr = NULL;
}

int main(int argc, char *argv[])
{
    creat_0100();
    creat_0200();
    creat_0300();
    return t_status;
}
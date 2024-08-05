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
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "functionalext.h"

/**
 * @tc.name      : shm_open_0100
 * @tc.desc      : Test if shm_open can successfully create and open a shared memory object.
 * @tc.level     : level 0
 */
void shm_open_0100()
{
    const char* shm_name = "/test_shm";
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    EXPECT_NE("shm_open_0100", fd, -1);
    close(fd);
    shm_unlink(shm_name);
}

/**
 * @tc.name      : shm_open_0200
 * @tc.desc      : Test if shm_open can successfully open an existing shared memory object without the O_CREAT flag.
 * @tc.level     : level 0
 */
void shm_open_0200()
{
    const char* shm_name = "/test_shm_0200";
    int fd1 = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    EXPECT_NE("shm_open_0200", fd1, -1);
    close(fd1);
    int fd2 = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
    EXPECT_NE("shm_open_0200", fd2, -1);
    close(fd2);
    shm_unlink(shm_name);
}


/**
 * @tc.name      : shm_open_0300
 * @tc.desc      : Test if shm_open returns an error when the shared memory 
 * object already exists and O_CREAT and O_EXCL flags are set.
 * @tc.level     : level 0
 */
void shm_open_0300()
{
    const char* shm_name = "/test_shm_0300";
    int fd1 = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    EXPECT_NE("shm_open_0200", fd1, -1);
    close(fd1);
    int fd2 = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    EXPECT_EQ("shm_open_0300", fd2, -1);
    if (fd2 != -1) {
        close(fd2);
    }
    shm_unlink(shm_name);
}

/**
 * @tc.name      : shm_open_0400
 * @tc.desc      : Test if shm_open returns an error when trying to open a non-existent
 * shared memory object without the O_CREAT flag.
 * @tc.level     : level 0
 */
void shm_open_0400()
{
    const char* shm_name = "/test_shm_0400";
    int fd = shm_open(shm_name, O_RDWR, S_IRUSR | S_IWUSR);
    EXPECT_EQ("shm_open_0400", fd, -1);
    if (fd != -1) {
        close(fd);
    } 
}


int main()
{
    shm_open_0100();
    shm_open_0200();
    shm_open_0300();
    shm_open_0400();
    return t_status;
}

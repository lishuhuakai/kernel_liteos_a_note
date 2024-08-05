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
#include <sys/mman.h>
#include "functionalext.h"

/**
 * @tc.name      : memfd_create_0100
 * @tc.desc      : Process memfd_create success.
 * @tc.level     : Level 0
 */
void memfd_create_0100(void)
{
    char tmpfile[] = "/data/memfd_create_0100.txt";
    int fd = memfd_create(tmpfile, 0);
    EXPECT_TRUE("memfd_create_0100", fd != -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("memfd_create_0100", cnt == strlen(tmpfile));
        close(fd);
    }
}

/**
 * @tc.name: memfd_create_0200
 * @tc.desc: Process memfd_create fail. because param is error.
 * @tc.level: Level 2
 */
void memfd_create_0200(void)
{
    char tmpfile[] = "/data/memfd_create_0200.txt";
    int fd = memfd_create(tmpfile, -1);
    EXPECT_TRUE("memfd_create_0200", fd == -1);
    if (fd != -1) {
        int cnt = write(fd, tmpfile, strlen(tmpfile));
        EXPECT_TRUE("memfd_create_0200", cnt == strlen(tmpfile));
        close(fd);
    }
}

int main(void)
{
    memfd_create_0100();
    memfd_create_0200();
    return t_status;
}
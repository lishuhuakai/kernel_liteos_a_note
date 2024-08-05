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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "test.h"

/**
 * @tc.name      : ttyname_r_0100
 * @tc.desc      : Call ttyname_r to get the file name through the file handle
 * @tc.level     : Level 0
 */
void ttyname_r_0100(void)
{
    int fd = open("/data/tempfile_forttyname.txt", O_CREAT, TEST_MODE);
    char buf[128];
    int want = 25;
    int result = ttyname_r(fd, buf, sizeof(buf));
    if (result != want) {
        t_error("%s ttyname_r error get result is %d are not want 25\n", __func__, result);
    }
    close(fd);
    unlink("/data/tempfile_forttyname.txt");
}

/**
 * @tc.name      : ttyname_r_0200
 * @tc.desc      : Test the ttyname_r result when a non-existing file is passed in
 * @tc.level     : Level 1
 */
void ttyname_r_0200(void)
{
    int fd = open("/dev/null", O_WRONLY);
    char buf[1];
    int result = ttyname_r(fd, buf, sizeof(buf));
    if (result != errno) {
        t_error("%s ttyname_r error get result is %d are not want errno\n", __func__, result);
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    ttyname_r_0100();
    ttyname_r_0200();
    return t_status;
}
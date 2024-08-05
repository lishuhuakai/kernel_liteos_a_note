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
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "test.h"

/**
 * @tc.name      : ttyname_0100
 * @tc.desc      : Test the ttyname method to get the terminal name through the file handle
 * @tc.level     : Level 0
 */
void ttyname_0100(void)
{
    char *file = "/dev/tty";
    int fd = open(file, O_RDONLY);
    char *result = ttyname(fd);
    if (strcmp(result, file) != 0) {
        t_error("%s ttyname get result is %s are not want %s\n", __func__, result, file);
    }
}

/**
 * @tc.name      : ttyname_0200
 * @tc.desc      : Test the return value of ttyname when a non-existing terminal file handle is passed in
 * @tc.level     : Level 1
 */
void ttyname_0200(void)
{
    char *file = "/unexit";
    int fd = open(file, O_RDONLY);
    char *result = ttyname(fd);
    if (result) {
        t_error("%s ttyname get result is %s are not null\n", __func__, result);
    }
}

int main(void)
{
    ttyname_0100();
    ttyname_0200();
    return t_status;
}
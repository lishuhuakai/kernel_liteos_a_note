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
#include "test.h"

/**
 * @tc.name      : symlink_0100
 * @tc.desc      : Test the symlink function to create a new file link
 * @tc.level     : Level 0
 */
void symlink_0100(void)
{
    char *file_name = "/data/temp.txt";
    char *link_name = "/data/linktemp.txt";
    FILE *f = fopen(file_name, "w");
    fclose(f);
    int result = symlink(file_name, link_name);
    if (result != 0) {
        t_error("%s symlink get result is %d are not 0\n", __func__, result);
    }
    if (access(link_name, 0) != 0) {
        t_error("%s symlink error link not exist\n", __func__);
    }
    unlink(file_name);
    unlink(link_name);
}

/**
 * @tc.name      : symlink_0200
 * @tc.desc      : old file does not exist
 * @tc.level     : Level 1
 */
void symlink_0200(void)
{
    char *file_name = "/data/temp.txt";
    char *link_name = "/data/linktemp.txt";
    int result = symlink(file_name, link_name);
    if (result != 0) {
        t_error("%s symlink get result is %d are not 0\n", __func__, result);
    }
    if (access(link_name, 0) == 0) {
        t_error("%s symlink error link exist\n", __func__);
    }
    unlink(link_name);
}

int main(int argc, char *argv[])
{
    symlink_0100();
    symlink_0200();
    return t_status;
}
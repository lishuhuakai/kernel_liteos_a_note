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

#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getpagesize_0100
 * @tc.desc      : The call is successful and the size of the page can be obtained
 * @tc.level     : Level 0
 */
void getpagesize_0100()
{
    char list[100];
    int page = getpagesize();
    system("getconf PAGE_SIZE > /data/1.txt");
    FILE *fd;
    fd = fopen("/data/1.txt", "r");
    fread(list, sizeof(list) / sizeof(list[0]), sizeof(*list), fd);
    EXPECT_EQ("getpagesize_0100", page, atoi(list));
    remove("/data/1.txt");
}

int main(int argc, char *argv[])
{
    getpagesize_0100();
    return t_status;
}
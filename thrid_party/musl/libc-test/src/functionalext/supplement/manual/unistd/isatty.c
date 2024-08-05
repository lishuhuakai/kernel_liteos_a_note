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
#include <unistd.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : isatty_0100
 * @tc.desc      : Verify that the file is a terminal (parameters are valid)
 * @tc.level     : Level 0
 */
void isatty_0100()
{
    int result = isatty(fileno(stdout));
    EXPECT_EQ("isatty_0100", result, 1);
}

/**
 * @tc.name      : isatty_0200
 * @tc.desc      : Verify that the file is not a terminal (parameter invalid)
 * @tc.level     : Level 2
 */
void isatty_0200()
{
    int fd = open("/data/readtest.txt", O_RDWR | O_CREAT, TEST_MODE);
    int result = isatty(fd);
    EXPECT_EQ("isatty_0200", result, 0);
    close(fd);
    remove("/data/readtest.txt");
}

TEST_FUN G_Fun_Array[] = {
    isatty_0100,
    isatty_0200,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
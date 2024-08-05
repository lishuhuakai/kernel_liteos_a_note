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

#include <arpa/inet.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();

/**
 * @tc.name      : inet_addr_0100
 * @tc.desc      : The parameter is valid and converts an IP address string into a network binary number
 * @tc.level     : Level 0
 */
void inet_addr_0100()
{
    char buff[] = "127.0.0.1";
    int result = inet_addr(buff);
    EXPECT_NE("inet_addr_0100", result, INADDR_NONE);
}

/**
 * @tc.name      : inet_addr_0200
 * @tc.desc      : The p argument is invalid and has a negative number. An IP address string cannot
 *                 be converted to a network binary number
 * @tc.level     : Level 2
 */
void inet_addr_0200()
{
    char buff[] = "-127.0.0.1";
    int result = inet_addr(buff);
    EXPECT_EQ("inet_addr_0200", result, INADDR_NONE);
}

/**
 * @tc.name      : inet_addr_0300
 * @tc.desc      : The p argument is invalid. A number greater than 255 exists. An IP address string cannot be
 *                 converted to a network binary number
 * @tc.level     : Level 2
 */
void inet_addr_0300()
{
    char buff[] = "127.0.256.1";
    int result = inet_addr(buff);
    EXPECT_EQ("inet_addr_0300", result, INADDR_NONE);
}

TEST_FUN G_Fun_Array[] = {
    inet_addr_0100,
    inet_addr_0200,
    inet_addr_0300,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
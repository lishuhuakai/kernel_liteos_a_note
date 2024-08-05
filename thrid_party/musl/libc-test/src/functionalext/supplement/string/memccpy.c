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

#include <string.h>
#include "functionalext.h"

#define TEST_BUFFER_SIZE 128
#define TEST_DATA_LEN 2

/**
 * @tc.name      : memccpy_0100
 * @tc.desc      : Copies a string until spaces or data is greater than the specified length
 * @tc.level     : Level 0
 */
void memccpy_0100(void)
{
    const char src[TEST_BUFFER_SIZE] = "musl test";
    char dest[TEST_BUFFER_SIZE] = "";
    int i;
    void *rev = memccpy(dest, src, ' ', TEST_DATA_LEN);
    EXPECT_PTREQ("memccpy_0100", rev, NULL);
    EXPECT_EQ("memccpy_0100", strlen(dest), TEST_DATA_LEN);
    for (i = 0; i < TEST_DATA_LEN; i++) {
        EXPECT_EQ("memccpy_0100", dest[i], src[i]);
    }

    memset(dest, 0x0, sizeof(dest));
    rev = memccpy(dest, src, ' ', sizeof(src));
    EXPECT_PTRNE("memccpy_0100", rev, NULL);
    EXPECT_EQ("memccpy_0100", strlen(dest), strlen("musl "));
    for (i = 0; i < strlen("musl "); i++) {
        EXPECT_EQ("memccpy_0100", dest[i], src[i]);
    }
}

/**
 * @tc.name      : memccpy_0200
 * @tc.desc      : Use a character not contained in the original string as the end position
 *                 for processing the copied string
 * @tc.level     : Level 0
 */
void memccpy_0200(void)
{
    const char src[TEST_BUFFER_SIZE] = "musl test";
    char dest[TEST_BUFFER_SIZE] = "";
    int i;
    void *rev = memccpy(dest, src, 'A', strlen(src));
    EXPECT_PTREQ("memccpy_0200", rev, NULL);
    EXPECT_STREQ("memccpy_0200", dest, src);
}

/**
 * @tc.name      : memccpy_0300
 * @tc.desc      : The length of the copied string is 0
 * @tc.level     : Level 2
 */
void memccpy_0300(void)
{
    const char src[TEST_BUFFER_SIZE] = "musl test";
    char dest[TEST_BUFFER_SIZE] = "";
    int i;
    void *rev = memccpy(dest, src, ' ', 0);
    EXPECT_PTREQ("memccpy_0300", rev, NULL);
    EXPECT_EQ("memccpy_0300", strlen(dest), 0);
}

int main(void)
{
    memccpy_0100();
    memccpy_0200();
    memccpy_0300();
    return t_status;
}
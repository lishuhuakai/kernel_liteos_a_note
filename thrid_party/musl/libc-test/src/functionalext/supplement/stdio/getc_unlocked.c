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
#include "functionalext.h"

const int SIZE = 97;

/**
 * @tc.name      : getc_unlocked_0100
 * @tc.desc      : The parameter is valid, and the specified character can be obtained.
 * @tc.level     : Level 0
 */
void getc_unlocked_0100(void)
{
    const char *ptr = "getc_unlockedtest.txt";
    FILE *fptr = fopen(ptr, "wr+");
    EXPECT_TRUE("getc_unlocked_0100", fptr != NULL);
    const char *wstr = "a";
    fwrite(wstr, sizeof(char), strlen(wstr), fptr);
    fseek(fptr, 0, SEEK_SET);
    int ret = getc_unlocked(fptr);
    EXPECT_EQ("getc_unlocked_0100", ret, SIZE);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : getc_unlocked_0200
 * @tc.desc      : The parameter f is invalid, and the specified character cannot be obtained.
 * @tc.level     : Level 2
 */
void getc_unlocked_0200(void)
{
    const char *ptr = "getc_unlockedtest.txt";
    FILE *fptr = fopen(ptr, "wr+");
    EXPECT_TRUE("getc_unlocked_0200", fptr != NULL);
    fclose(fptr);
    fptr = fopen(ptr, "r");
    EXPECT_TRUE("getc_unlocked_0200", fptr != NULL);
    int ret = getc_unlocked(fptr);
    EXPECT_EQ("getc_unlocked_0200", ret, EOF);
    fclose(fptr);
    remove(ptr);
    fptr = NULL;
    ptr = NULL;
}

int main(int argc, char *argv[])
{
    getc_unlocked_0100();
    getc_unlocked_0200();
    return t_status;
}
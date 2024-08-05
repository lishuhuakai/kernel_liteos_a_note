/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#include <stdio.h>
#include <stdlib.h>
#include "functionalext.h"

const int32_t CREAT_MODE = 666;

/**
 * @tc.name      : rewind_0100
 * @tc.desc      : Verify rewind process success
 * @tc.level     : Level 0
 */
void rewind_0100(void)
{
    int32_t ret = creat("/data/rewind_0100.txt", CREAT_MODE);
    if (ret < 0) {
        EXPECT_MT("rewind_0100", ret, 0);
        return;
    }

    FILE *fptr = fopen("/data/rewind_0100.txt", "w");
    EXPECT_PTRNE("rewind_0100", fptr, NULL);
    char str[] = "test";
    fwrite(str, 1, sizeof(str), fptr);
    ret = fclose(fptr);
    EXPECT_EQ("rewind_0100", ret, 0);

    fptr = fopen("/data/rewind_0100.txt", "r");
    EXPECT_PTRNE("rewind_0100", fptr, NULL);
    int ch;
    while ( (ch = fgetc(fptr)) != EOF ){
        break;
    }

    rewind(fptr);
    EXPECT_FALSE("rewind_0100", feof(fptr));
    fclose(fptr);
    remove("/data/rewind_0100.txt");
}

int main(void)
{
    rewind_0100();
    return t_status;
}

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

const int32_t PUTC_RET = 112;
const int32_t CREAT_MODE = 666;

/**
 * @tc.name      : putc_0100
 * @tc.desc      : Verify putc process success
 * @tc.level     : Level 0
 */
void putc_0100(void)
{
    int32_t ret = creat("putc_0100.txt", CREAT_MODE);
    if (ret < 0) {
        EXPECT_MT("pread_0100", ret, 0);
        return;
    }
    FILE *fptr = fopen("putc_0100.txt", "w");
    if (!fptr) {
        EXPECT_PTRNE("putc_0100", fptr, NULL);
        return;
    }
    EXPECT_PTRNE("putc_0100", fptr, NULL);
    EXPECT_EQ("putc_0100", putc('p', fptr), PUTC_RET);
    ret = fclose(fptr);
    EXPECT_EQ("putc_0100", ret, 0);
    remove("putc_0100.txt");
}

/**
 * @tc.name      : putc_0200
 * @tc.desc      : Verify putc process failed
 * @tc.level     : Level 2
 */
void putc_0200(void)
{
    int ret = creat("putc_0200.txt", CREAT_MODE);
    if (ret < 0) {
        EXPECT_MT("pread_0200", ret, 0);
        return;
    }
    FILE *fptr = fopen("putc_0200.txt", "r");
    if (!fptr) {
        EXPECT_PTRNE("putc_0200", fptr, NULL);
        return;
    }
    EXPECT_EQ("putc_0200", putc('p', fptr), EOF);
    ret = fclose(fptr);
    EXPECT_EQ("putc_0100", ret, 0);
    remove("putc_0200.txt");
}

int main(void)
{
    putc_0100();
    putc_0200();
    return t_status;
}

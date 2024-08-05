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
#include <wchar.h>
#include "functionalext.h"

const int32_t PUTC_RET = 112;
const int32_t CREAT_MODE = 666;

/**
 * @tc.name      : putwc_0100
 * @tc.desc      : Verify putwc process success
 * @tc.level     : Level 0
 */
void putwc_0100(void)
{
    int32_t ret = creat("putwc_0100.txt", CREAT_MODE);
    if (ret < 0) {
        EXPECT_MT("putwc_0100", ret, 0);
        return;
    }
    FILE *fptr = fopen("putwc_0100.txt", "w");
    EXPECT_PTRNE("putwc_0100", fptr, NULL);
    wchar_t wc = L'p';
    ret = putwc(wc, fptr);
    EXPECT_EQ("putwc_0100", ret, PUTC_RET);
    ret = fclose(fptr);
    EXPECT_EQ("putwc_0100", ret, 0);
    remove("putwc_0100.txt");
}

/**
 * @tc.name       : putwc_0200
 * @tc.desc       : Verify fputc process failed.
 * @tc.level      : level 2.
 */
void putwc_0200(void)
{
    int32_t ret = creat("putwc_0200.txt", CREAT_MODE);
    if (ret < 0) {
        EXPECT_MT("putwc_0200", ret, 0);
        return;
    }
    FILE *fptr = fopen("putwc_0200.txt", "r");
    EXPECT_PTRNE("putwc_0200", fptr, NULL);
    wchar_t wc = L'p';
    ret = putwc(wc, fptr);
    EXPECT_EQ("putwc_0200", ret, WEOF);
    ret = fclose(fptr);
    EXPECT_EQ("putwc_0200", ret, 0);
    remove("putwc_0200.txt");
}

int main(void)
{
    putwc_0100();
    putwc_0200();
    return t_status;
}

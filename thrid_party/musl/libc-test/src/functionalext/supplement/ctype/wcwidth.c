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
#include <wchar.h>
#include <wctype.h>
#include "test.h"

const int INPUT_VALUE[12] = {
    0x0300, 0x20dd, 0x00ad, 0x200b, 0x4e00, 0x9fff, 0x3400, 0x4dbf, 0x20000, 0x2a6df, 0xac00, 0xd7a3};
const int RESULT_VALUE = 2;

/**
 * @tc.name      : wcwidth_0100
 * @tc.desc      : Test that the wcwidth method returns the desired number of columns
 * @tc.level     : Level 0
 */
void wcwidth_0100(void)
{
    int result = wcwidth(0);
    if (result != 0) {
        t_error("%s wcwidth get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_0200
 * @tc.desc      : Combining grave
 * @tc.level     : Level 1
 */
void wcwidth_0200(void)
{
    int result = wcwidth(INPUT_VALUE[0]);
    if (result != 0) {
        t_error("%s wcwidth get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_0300
 * @tc.desc      : Combining enclosing circle
 * @tc.level     : Level 1
 */
void wcwidth_0300(void)
{
    int result = wcwidth(INPUT_VALUE[1]);
    if (result != 0) {
        t_error("%s wcwidth get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_0400
 * @tc.desc      : Soft hyphen (SHY)
 * @tc.level     : Level 1
 */
void wcwidth_0400(void)
{
    int result = wcwidth(INPUT_VALUE[2]);
    if (result != 1) {
        t_error("%s wcwidth get result is %d are not want 1\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_0500
 * @tc.desc      : Zero width space
 * @tc.level     : Level 1
 */
void wcwidth_0500(void)
{
    int result = wcwidth(INPUT_VALUE[3]);
    if (result != 0) {
        t_error("%s wcwidth get result is %d are not want 0\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_0600
 * @tc.desc      :  Start of CJK unified block
 * @tc.level     : Level 1
 */
void wcwidth_0600(void)
{
    int result = wcwidth(INPUT_VALUE[4]);
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_0700
 * @tc.desc      : End of CJK unified block
 * @tc.level     : Level 1
 */
void wcwidth_0700(void)
{
    int result = wcwidth(INPUT_VALUE[5]);
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_0800
 * @tc.desc      : Start of CJK extension A block.
 * @tc.level     : Level 1
 */
void wcwidth_0800(void)
{
    int result = wcwidth(INPUT_VALUE[6]);
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_0900
 * @tc.desc      : End of CJK extension A block.
 * @tc.level     : Level 1
 */
void wcwidth_0900(void)
{
    int result = wcwidth(INPUT_VALUE[7]);
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_1000
 * @tc.desc      : Start of CJK extension B block.
 * @tc.level     : Level 1
 */
void wcwidth_1000(void)
{
    int result = wcwidth(INPUT_VALUE[8]);
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_1100
 * @tc.desc      : End of CJK extension B block.
 * @tc.level     : Level 1
 */
void wcwidth_1100(void)
{
    int result = wcwidth(INPUT_VALUE[9]);
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_1200
 * @tc.desc      : Start of block
 * @tc.level     : Level 1
 */
void wcwidth_1200(void)
{
    int result = wcwidth(INPUT_VALUE[10]);
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_1300
 * @tc.desc      : End of defined code points in Unicode 7
 * @tc.level     : Level 1
 */
void wcwidth_1300(void)
{
    int result = wcwidth(INPUT_VALUE[11]);
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_1400
 * @tc.desc      : Korean "crying" emoticon.
 * @tc.level     : Level 1
 */
void wcwidth_1400(void)
{
    int result = wcwidth(L'ㅜ');
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

/**
 * @tc.name      : wcwidth_1500
 * @tc.desc      : Korean "laughing" emoticon..
 * @tc.level     : Level 1
 */
void wcwidth_1500(void)
{
    int result = wcwidth(L'ㅋ');
    if (result != RESULT_VALUE) {
        t_error("%s wcwidth get result is %d are not want 2\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    wcwidth_0100();
    wcwidth_0200();
    wcwidth_0300();
    wcwidth_0400();
    wcwidth_0500();
    wcwidth_0600();
    wcwidth_0700();
    wcwidth_0800();
    wcwidth_0900();
    wcwidth_1000();
    wcwidth_1100();
    wcwidth_1200();
    wcwidth_1300();
    wcwidth_1400();
    wcwidth_1500();
    return t_status;
}
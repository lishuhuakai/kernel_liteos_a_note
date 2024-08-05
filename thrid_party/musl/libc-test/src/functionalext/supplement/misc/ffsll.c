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

#include "functionalext.h"

/**
 * @tc.name      : ffsll_0100
 * @tc.desc      : Verify to find the first set value of a value of type long long
 *                 (valid for each argument, 0, bit 1)
 * @tc.level     : Level 0
 */
void ffsll_0100(void)
{
    int result = ffsll(0);
    EXPECT_EQ("ffsll_0100", result, 0);
}

/**
 * @tc.name      : ffsll_0200
 * @tc.desc      : Verify to find the first set value of a value of type long long
 *                 (valid for each argument, 0x8000000,bit 1)
 * @tc.level     : Level 0
 */
void ffsll_0200(void)
{
    int result = ffsll(0x8000000);
    EXPECT_EQ("ffsll_0200", result, 28);
}

/**
 * @tc.name      : ffsll_0300
 * @tc.desc      : Verify to find the first set value of a value of type long long
 *                 (valid for each argument, 0x800000000,bit 1)
 * @tc.level     : Level 0
 */
void ffsll_0300(void)
{
    int result = ffsll(0x800000000);
    EXPECT_EQ("ffsll_0300", result, 36);
}

int main(int argc, char *argv[])
{
    ffsll_0100();
    ffsll_0200();
    ffsll_0300();
    return t_status;
}
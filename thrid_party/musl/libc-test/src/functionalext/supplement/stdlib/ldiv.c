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

#include <stdlib.h>
#include "functionalext.h"

#define TEST_LDIV_P_SEVENT 7
#define TEST_LDIV_P_FOUR 4
#define TEST_LDIV_P_THREE 3
#define TEST_LDIV_N_SEVENT (-7)
#define TEST_LDIV_N_FOUR (-4)
#define TEST_LDIV_N_THREE (-3)

/**
 * @tc.name      : ldiv_0100
 * @tc.desc      : Divide two long integers, return the quotient and remainder
 * @tc.level     : Level 0
 */
void ldiv_0100(void)
{
    ldiv_t ret;
    ret = ldiv(TEST_LDIV_P_SEVENT, TEST_LDIV_P_FOUR);
    EXPECT_EQ("ldiv_0100", ret.quot, 1);
    EXPECT_EQ("ldiv_0100", ret.rem, TEST_LDIV_P_THREE);

    ret = ldiv(TEST_LDIV_P_SEVENT, TEST_LDIV_N_FOUR);
    EXPECT_EQ("ldiv_0100", ret.quot, -1);
    EXPECT_EQ("ldiv_0100", ret.rem, TEST_LDIV_P_THREE);

    ret = ldiv(TEST_LDIV_N_SEVENT, TEST_LDIV_P_FOUR);
    EXPECT_EQ("ldiv_0100", ret.quot, -1);
    EXPECT_EQ("ldiv_0100", ret.rem, TEST_LDIV_N_THREE);

    ret = ldiv(TEST_LDIV_N_SEVENT, TEST_LDIV_N_FOUR);
    EXPECT_EQ("ldiv_0100", ret.quot, 1);
    EXPECT_EQ("ldiv_0100", ret.rem, TEST_LDIV_N_THREE);
}

int main(void)
{
    ldiv_0100();
    return t_status;
}
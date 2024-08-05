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

#include <limits.h>
#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : localeconv_0100
 * @tc.desc      : Asserts whether the localeconv function returns a value that is expected by default
 * @tc.level     : Level 0
 */
void localeconv_0100(void)
{
    struct lconv *pconv = localeconv();
    if (pconv == NULL) {
        EXPECT_PTRNE("localeconv_0100", pconv, NULL);
        return;
    }
    EXPECT_STREQ("localeconv_0100", ".", pconv->decimal_point);
    EXPECT_STREQ("localeconv_0100", "", pconv->thousands_sep);
    EXPECT_STREQ("localeconv_0100", "", pconv->grouping);
    EXPECT_STREQ("localeconv_0100", "", pconv->int_curr_symbol);
    EXPECT_STREQ("localeconv_0100", "", pconv->currency_symbol);
    EXPECT_STREQ("localeconv_0100", "", pconv->mon_decimal_point);
    EXPECT_STREQ("localeconv_0100", "", pconv->mon_thousands_sep);
    EXPECT_STREQ("localeconv_0100", "", pconv->mon_grouping);
    EXPECT_STREQ("localeconv_0100", "", pconv->positive_sign);
    EXPECT_STREQ("localeconv_0100", "", pconv->negative_sign);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->int_frac_digits);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->frac_digits);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->p_cs_precedes);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->p_sep_by_space);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->n_cs_precedes);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->n_sep_by_space);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->p_sign_posn);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->n_sign_posn);
#ifdef __USE_ISOC99
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->int_p_cs_precedes);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->int_p_sep_by_space);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->int_n_cs_precedes);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->int_n_sep_by_space);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->int_p_sign_posn);
    EXPECT_EQ("localeconv_0100", CHAR_MAX, pconv->int_n_sign_posn);
#endif
}

int main(void)
{
    localeconv_0100();
    return t_status;
}
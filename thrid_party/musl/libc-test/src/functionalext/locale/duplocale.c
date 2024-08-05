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

#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : duplocale_0100
 * @tc.desc      : Asserts whether duplocale returns a null value for the global locale
 * @tc.level     : Level 0
 */
void duplocale_0100(void)
{
    char *lo = setlocale(LC_ALL, "C");
    if (!lo) {
        EXPECT_PTRNE("duplocale_0100", lo, NULL);
        return;
    }

    locale_t newlocale_ = duplocale(LC_GLOBAL_LOCALE);
    EXPECT_TRUE("duplocale_0100", newlocale_);
    if (newlocale_) {
        freelocale(newlocale_);
        newlocale_ = NULL;
    }
}

/**
 * @tc.name      : duplocale_0200
 * @tc.desc      : Asserts that duplocale jumps first to the global locale and then to the custom locale,
 * expecting that the results of the two calls are not equal
 * @tc.level     : Level 0
 */
void duplocale_0200(void)
{
    char *lo = setlocale(LC_ALL, "C");
    if (!lo) {
        EXPECT_PTRNE("duplocale_0200", lo, NULL);
        return;
    }

    locale_t global = duplocale(LC_GLOBAL_LOCALE);
    EXPECT_PTRNE("duplocale_0200", global, NULL);

    locale_t newlocale_ = newlocale(LC_ALL_MASK, "en_US", NULL);
    EXPECT_PTRNE("duplocale_0200", newlocale_, NULL);

    locale_t clonelocale = duplocale(newlocale_);
    EXPECT_PTRNE("duplocale_0200", clonelocale, NULL);
    EXPECT_PTRNE("duplocale_0200", clonelocale, global);

    if (clonelocale) {
        freelocale(clonelocale);
        clonelocale = NULL;
    }
    if (newlocale_) {
        freelocale(newlocale_);
        newlocale_ = NULL;
    }
    if (global) {
        freelocale(global);
        global = NULL;
    }
}

int main(void)
{
    duplocale_0100();
    duplocale_0200();
    return t_status;
}
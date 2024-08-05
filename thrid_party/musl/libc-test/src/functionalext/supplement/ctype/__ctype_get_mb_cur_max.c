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

#include <locale.h>
#include <stdlib.h>
#include "functionalext.h"

typedef void (*TEST_FUN)();
const ssize_t COUNT_ONE = 1;
const ssize_t COUNT_FOUR = 4;

/**
 * @tc.name      : __ctype_get_mb_cur_max_0100
 * @tc.desc      : Gets the maximum length of a multi-byte character (Western Language - Latin 1)
 * @tc.level     : Level 0
 */
void __ctype_get_mb_cur_max_0100()
{
    setlocale(LC_ALL, ".1252");
    ssize_t result;
    result = __ctype_get_mb_cur_max();
    EXPECT_TRUE("__ctype_get_mb_cur_max_0100", result > 0);
}

/**
 * @tc.name      : __ctype_get_mb_cur_max_0200
 * @tc.desc      : Gets the maximum length of a multi-byte character (UTF8)
 * @tc.level     : Level 0
 */
void __ctype_get_mb_cur_max_0200()
{
    setlocale(LC_CTYPE, "zh_CN.utf8");
    ssize_t result;
    result = __ctype_get_mb_cur_max();
    EXPECT_EQ("__ctype_get_mb_cur_max_0200", result, COUNT_FOUR);
}

/**
 * @tc.name      : __ctype_get_mb_cur_max_0300
 * @tc.desc      : Gets the maximum length of a multi-byte character (Chinese GBK)
 * @tc.level     : Level 0
 */
void __ctype_get_mb_cur_max_0300()
{
    setlocale(LC_ALL, ".936");
    ssize_t result;
    result = __ctype_get_mb_cur_max();
    EXPECT_EQ("__ctype_get_mb_cur_max_0300", result, COUNT_FOUR);
}

TEST_FUN G_Fun_Array[] = {
    __ctype_get_mb_cur_max_0100,
    __ctype_get_mb_cur_max_0200,
    __ctype_get_mb_cur_max_0300,
};

int main(int argc, char *argv[])
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
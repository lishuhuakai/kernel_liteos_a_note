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
#include "test.h"
#include "strops.h"
#include "functionalext.h"

typedef void (*TEST_FUN)(void);

/**
 * @tc.name      : strops_test_0010
 * @tc.desc      : strlwc test args null
 * @tc.level     : Level 2
 */
void strops_test_0010(void)
{
    char *str = NULL; 
    strlwc(str);
    EXPECT_EQ(__FUNCTION__, str, NULL);
}

/**
 * @tc.name      : strops_test_0020
 * @tc.desc      : Test strlwc with normal input 
 * @tc.level     : Level 0
 */
void strops_test_0020(void)
{
    char tmp[4] = "Abc";
    tmp[3] = '\0';
    strlwc(tmp);

    EXPECT_EQ(__FUNCTION__, strcmp(tmp, "abc"), 0);
}

/**
 * @tc.name      : strops_test_0030
 * @tc.desc      : strlist_set test arg is NULL
 * @tc.level     : Level 2
 */
void strops_test_0030(void)
{
    char *str = NULL; 
    strlist_set(NULL, NULL);

    EXPECT_EQ(__FUNCTION__, str, NULL);
}

/**
 * @tc.name      : strops_test_0040
 * @tc.desc      : Test strlist_set with normal input 
 * @tc.level     : Level 0
 */
void strops_test_0040(void)
{
    strlist list;
    list.size = 1;
    list.num = 1;
    strlist_set(&list, "abc");

    EXPECT_EQ(__FUNCTION__, list.size, 2);
}

/**
 * @tc.name      : strops_test_0050
 * @tc.desc      : strtrim test arg is NULL
 * @tc.level     : Level 2
 */
void strops_test_0050(void)
{
    size_t ret = strtrim(NULL);
    EXPECT_EQ(__FUNCTION__, ret, 0);
}

TEST_FUN G_Fun_Array[] = {
    strops_test_0010,
    strops_test_0020,
    strops_test_0030,
    strops_test_0040,
    strops_test_0050
};

int main(void)
{
    int num = sizeof(G_Fun_Array) / sizeof(TEST_FUN);
    for (int pos = 0; pos < num; ++pos) {
        G_Fun_Array[pos]();
    }

    return t_status;
}
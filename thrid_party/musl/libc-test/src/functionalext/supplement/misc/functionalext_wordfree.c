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

#include <wordexp.h>
#include "functionalext.h"

/**
 * @tc.name      : wordfree_0100
 * @tc.desc      : Verify that after wordexp operation, wordfree releases the resources properly.
 * @tc.level     : level 0
 */
void wordfree_0100()
{
    wordexp_t we;
    int ret = wordexp("ls -l", &we, 0);
    EXPECT_EQ("wordfree_0100", ret, 0);
    EXPECT_EQ("wordfree_0100", we.we_wordc > 0, 1);
    EXPECT_EQ("wordfree_0100", we.we_wordv != NULL, 1);
    wordfree(&we);
    EXPECT_EQ("wordfree_0100", we.we_wordv, (char **)0);
    EXPECT_EQ("wordfree_0100", we.we_wordc, 0);
}

/**
 * @tc.name      : wordfree_0200
 * @tc.desc      : Verify that wordfree can handle and release an uninitialized wordexp_t structure.
 * @tc.level     : level 0
 */
void wordfree_0200()
{
    wordexp_t we = {0};
    wordfree(&we);
    EXPECT_EQ("wordfree_0200", we.we_wordv, (char **)0);
    EXPECT_EQ("wordfree_0200", we.we_wordc, 0);
}

int main()
{
    wordfree_0100();
    wordfree_0200();
    return t_status;
}

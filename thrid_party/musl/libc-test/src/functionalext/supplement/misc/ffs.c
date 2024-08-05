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
 * @tc.name      : ffs_0100
 * @tc.desc      : Validation looks for first set value in integer (parameter is 0).
 * @tc.level     : Level 0
 */
void ffs_0100(void)
{
    int result = ffs(0);
    EXPECT_EQ("ffs_0100", result, 0);
}

/**
 * @tc.name      : ffs_0200
 * @tc.desc      : Validation looks for first set value in integer (parameter is 1).
 * @tc.level     : Level 0
 */
void ffs_0200(void)
{
    int result = ffs(1);
    EXPECT_EQ("ffs_0200", result, 1);
}

/**
 * @tc.name      : ffs_0300
 * @tc.desc      : Validation looks for first set value in integer (parameter is 2).
 * @tc.level     : Level 0
 */
void ffs_0300(void)
{
    int result = ffs(2);
    EXPECT_EQ("ffs_0300", result, 2);
}

/**
 * @tc.name      : ffs_0400
 * @tc.desc      : Validation looks for first set value in integer (parameter is 3).
 * @tc.level     : Level 0
 */
void ffs_0400(void)
{
    int result = ffs(3);
    EXPECT_EQ("ffs_0400", result, 1);
}

/**
 * @tc.name      : ffs_0500
 * @tc.desc      : Validation looks for first set value in integer (parameter is 255).
 * @tc.level     : Level 0
 */
void ffs_0500(void)
{
    int result = ffs(128);
    EXPECT_EQ("ffs_0500", result, 8);
}

int main(int argc, char *argv[])
{
    ffs_0100();
    ffs_0200();
    ffs_0300();
    ffs_0400();
    ffs_0500();
    return t_status;
}
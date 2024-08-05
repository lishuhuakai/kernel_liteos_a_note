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

/**
 * @tc.name      : mktemp_0100
 * @tc.desc      : Create a temporary file with the correct parameters
 * @tc.level     : Level 0
 */
void mktemp_0100(void)
{
    char temp_file[] = "test-XXXXXX";
    char *ret = mktemp(temp_file);
    EXPECT_PTRNE("mktemp_0100", ret, NULL);
    if (ret) {
        EXPECT_NE("mktemp_0100", ret[0], 0);
    }
}

/**
 * @tc.name      : mktemp_0200
 * @tc.desc      : Create temp file with wrong template parameter
 * @tc.level     : Level 2
 */
void mktemp_0200(void)
{
    char temp_file[] = "test";
    char *ret = mktemp(temp_file);
    EXPECT_PTRNE("mktemp_0200", ret, NULL);
    if (ret) {
        EXPECT_EQ("mktemp_0200", ret[0], 0);
    }
}

/**
 * @tc.name      : mktemp_0300
 * @tc.desc      : Create temp file with wrong template argument (wrong template length)
 * @tc.level     : Level 2
 */
void mktemp_0300(void)
{
    char temp_file[] = "test-XX";
    char *ret = mktemp(temp_file);
    EXPECT_PTRNE("mktemp_0300", ret, NULL);
    if (ret) {
        EXPECT_EQ("mktemp_0300", ret[0], 0);
    }
}

int main(void)
{
    mktemp_0100();
    mktemp_0200();
    mktemp_0300();
    return t_status;
}
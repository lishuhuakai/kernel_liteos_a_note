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
 * @tc.name      : mkdtemp_0100
 * @tc.desc      : Create a temporary directory with the correct parameters
 * @tc.level     : Level 0
 */
void mkdtemp_0100(void)
{
    char temp_dir[] = "test-XXXXXX";
    char *ret = mkdtemp(temp_dir);
    EXPECT_PTRNE("mkdtemp_0100", ret, NULL);
    if (ret) {
        EXPECT_NE("mkdtemp_0100", ret[0], CMPFLAG);
    }
}

/**
 * @tc.name      : mkdtemp_0200
 * @tc.desc      : Create temp directory with wrong template parameter
 * @tc.level     : Level 2
 */
void mkdtemp_0200(void)
{
    char temp_dir[] = "test";
    char *ret = mkdtemp(temp_dir);
    EXPECT_PTREQ("mkdtemp_0200", ret, NULL);
}

/**
 * @tc.name      : mkdtemp_0300
 * @tc.desc      : Create temp directory with wrong template argument (wrong template length)
 * @tc.level     : Level 2
 */
void mkdtemp_0300(void)
{
    char temp_dir[] = "test-XX";
    char *ret = mkdtemp(temp_dir);
    EXPECT_PTREQ("mkdtemp_0300", ret, NULL);
}

int main(void)
{
    mkdtemp_0100();
    mkdtemp_0200();
    mkdtemp_0300();
    return t_status;
}
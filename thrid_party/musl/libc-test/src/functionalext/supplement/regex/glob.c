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

#include <glob.h>
#include <sys/stat.h>
#include "functionalext.h"

/**
 * @tc.name      : glob_0100
 * @tc.desc      : Verify glob process success
 * @tc.level     : Level 0
 */
void glob_0100(void)
{
    glob_t buf;
    int ret = glob("./*", GLOB_NOSORT, NULL, &buf);
    EXPECT_EQ("glob_0100", ret, 0);
    globfree(&buf);
}

/**
 * @tc.name      : glob_0200
 * @tc.desc      : Verify glob process fail. when no found matches
 * @tc.level     : Level 2
 */
void glob_0200(void)
{
    glob_t buf;
    int ret = glob("./does-not-match", GLOB_NOSORT, NULL, &buf);
    EXPECT_EQ("glob_0100", ret, GLOB_NOMATCH);
    globfree(&buf);
}

int main(void)
{
    glob_0100();
    glob_0200();
    return t_status;
}

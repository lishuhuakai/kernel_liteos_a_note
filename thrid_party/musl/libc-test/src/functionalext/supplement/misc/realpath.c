/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : realpath_0100
 * @tc.desc      : Verify realpath process success
 * @tc.level     : Level 0
 */
void realpath_0100(void)
{
    char actualPath[4096];
    char *ret = realpath("/system/bin/../etc", actualPath);
    EXPECT_STREQ("realpath_0100", ret, "/system/etc");
    EXPECT_STREQ("realpath_0100", actualPath, "/system/etc");
}

/**
 * @tc.name      : realpath_0200
 * @tc.desc      : Verify realpath process success fail bacaus path is error
 * @tc.level     : Level 2
 */
void realpath_0200(void)
{
    char actualPath[4096] = {0};
    char *ret = realpath("/system/bin/../mypth", actualPath);
    EXPECT_PTREQ("realpath_0200", ret, NULL);
    ret = realpath("/system/bin/../etc", NULL);
    EXPECT_STREQ("realpath_0200", ret, "/system/etc");
}

int main(int argc, char *argv[])
{
    realpath_0100();
    realpath_0200();
    return t_status;
}

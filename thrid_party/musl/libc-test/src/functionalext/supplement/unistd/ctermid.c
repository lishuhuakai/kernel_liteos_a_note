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

#include "functionalext.h"

/**
 * @tc.name      : ctermid_0100
 * @tc.desc      : Verify the path name of the control terminal that gets the current process (parameter valid)
 * @tc.level     : Level 0
 */
void ctermid_0100(void)
{
    char buf[BUFSIZ];
    char *result = ctermid(buf);
    EXPECT_EQ("ctermid_0100", buf, result);
    EXPECT_STREQ("ctermid_0100", result, "/dev/tty");
}

/**
 * @tc.name      : ctermid_0200
 * @tc.desc      : Verify the default pathname of the control terminal that got the current process
 *                 (the s parameter is invalid)
 * @tc.level     : Level 1
 */
void ctermid_0200(void)
{
    char *result = ctermid(NULL);
    EXPECT_STREQ("ctermid_0200", result, "/dev/tty");
}

int main(int argc, char *argv[])
{
    ctermid_0100();
    ctermid_0200();
    return t_status;
}
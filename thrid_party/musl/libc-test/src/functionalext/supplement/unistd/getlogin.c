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

#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getlogin_0100
 * @tc.desc      : Successfully obtained the logged in username.
 * @tc.level     : Level 0
 */
void getlogin_0100(void)
{
    char *ret;
    ret = getlogin();
    if (ret == NULL) {
        int senv = setenv("LOGNAME", "newlogname", 1);
        EXPECT_EQ("getlogin_0100", senv, 0);
        ret = getlogin();
        EXPECT_TRUE("getlogin_0100", strcmp(ret, "newlogname") == 0);
        unsetenv("LOGNAME");
    } else {
        char *genv;
        genv = getenv("LOGNAME");
        EXPECT_TRUE("getlogin_0100", strcmp(ret, genv) == 0);
    }
}

int main(int argc, char *argv[])
{
    getlogin_0100();
    return t_status;
}
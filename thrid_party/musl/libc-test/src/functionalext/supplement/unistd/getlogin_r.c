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
 * @tc.name      : getlogin_r_0100
 * @tc.desc      : Successfully obtained the logged in username.
 * @tc.level     : Level 0
 */
void getlogin_r_0100(void)
{
    char login[256];
    int ret = getlogin_r(login, 256);
    if (ret == 6) {
        int senv = setenv("LOGNAME", "newlogname", 10);
        EXPECT_EQ("getlogin_0100", senv, 0);

        ret = getlogin_r(login, 256);
        EXPECT_EQ("getlogin_0100", ret, 0);
        unsetenv("LOGNAME");
    } else {
        EXPECT_EQ("getlogin_0100", ret, 0);
    }
}

/**
 * @tc.name      : getlogin_r_0200
 * @tc.desc      : The parameter is invalid. Failed to get the logged in user name.
 * @tc.level     : Level 2
 */
void getlogin_r_0200(void)
{
    int ret = 0;
    char login[256];
    ret = getlogin_r("login", 0);
    EXPECT_NE("getlogin_r_0200", ret, 0);
}

int main(int argc, char *argv[])
{
    getlogin_r_0100();
    getlogin_r_0200();
    return t_status;
}
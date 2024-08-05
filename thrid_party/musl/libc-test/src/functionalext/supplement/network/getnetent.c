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

#include <netdb.h>
#include "functionalext.h"

const int SUCCESS = 0;

/**
 * @tc.name      : getnetent_0100
 * @tc.desc      : Successfully obtained network information.
 * @tc.level     : Level 0
 */
void getnetent_0100(void)
{
    struct netent *ret;
    ret = getnetent();
    EXPECT_EQ("getnetent_0100", ret, 0);
    ret = NULL;
}

int main(int argc, char *argv[])
{
    getnetent_0100();
    return t_status;
}
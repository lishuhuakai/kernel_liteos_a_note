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

#include <fenv.h>
#include "functionalext.h"

/**
 * @tc.name      : feholdexcept_0100
 * @tc.desc      : The parameter is valid and can save the current floating point environment.
 * @tc.level     : Level 0
 */
void feholdexcept_0100(void)
{
    int ret = -1;
    fenv_t envp;
    ret = feholdexcept(&envp);
    EXPECT_EQ("feholdexcept_0100", ret, 0);
}

int main(int argc, char *argv[])
{
    feholdexcept_0100();
    return t_status;
}

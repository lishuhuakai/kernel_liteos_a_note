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

#include <net/if.h>
#include "functionalext.h"

/**
 * @tc.name      : if_nameindex_0100
 * @tc.desc      : Verify if_nameindex process success
 * @tc.level     : Level 0
 */
void if_nameindex_0100(void)
{
    struct if_nameindex *ifni = if_nameindex();
    EXPECT_PTRNE("if_nameindex_0100", ifni, NULL);
    if_freenameindex(ifni);
}

int main(void)
{
    if_nameindex_0100();
    return t_status;
}

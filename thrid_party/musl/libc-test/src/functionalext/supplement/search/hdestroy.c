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

#include <search.h>
#include "functionalext.h"

const size_t SIZE = 2;

/**
 * @tc.name      : hdestroy_0100
 * @tc.desc      : Verify hdestroy process success
 * @tc.level     : Level 0
 */
void hdestroy_0100(void)
{
    struct hsearch_data h_data = {};
    int ret;
    EXPECT_NE("hdestroy_0100", hcreate(SIZE), 0);
    hdestroy();
}

int main(void)
{
    hdestroy_0100();
    return t_status;
}

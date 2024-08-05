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
static char *data[] = {"hsearch_r_0100", "hsearch_r_0200"};

/**
 * @tc.name      : hsearch_r_0100
 * @tc.desc      : Verify hsearch_r process success
 * @tc.level     : Level 0
 */
void hsearch_r_0100(void)
{
    struct hsearch_data h_data = {};
    int ret;
    EXPECT_NE("hsearch_r_0100", hcreate_r(SIZE, &h_data), 0);
    ENTRY e, *ep;
    for (int i = 0; i < SIZE; i++) {
        e.key = data[i];
        e.data = (void *)&i;
        ret = hsearch_r(e, ENTER, &ep, &h_data);
        EXPECT_NE("hsearch_r_0100", ret, 0);
    }
    hdestroy_r(&h_data);
}

/**
 * @tc.name      : hsearch_r_0200
 * @tc.desc      : Verify hsearch_r process success
 * @tc.level     : Level 2
 */
void hsearch_r_0200(void)
{
    struct hsearch_data h_data = {};
    int ret;
    EXPECT_NE("hsearch_r_0100", hcreate_r(SIZE, &h_data), 0);
    ENTRY e, *ep;
    for (int i = 0; i < SIZE; i++) {
        e.key = data[i];
        ret = hsearch_r(e, FIND, &ep, &h_data);
        EXPECT_EQ("hsearch_r_0100", ret, 0);
    }
    hdestroy_r(&h_data);
}

int main(void)
{
    hsearch_r_0100();
    hsearch_r_0200();
    return t_status;
}

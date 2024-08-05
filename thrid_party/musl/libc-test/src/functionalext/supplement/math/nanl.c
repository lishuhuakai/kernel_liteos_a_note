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

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : nanl.
 * @tc.desc      : Verify nanl process success and return 0.
 * @tc.level     : level 0
 */
void nanl_0100(void)
{
    long double ret = nanl("");
    if (fabsl(ret - NAN) > LDBL_EPSILON) {
        t_error("nanl_0100 failed");
    }
}

int main(void)
{
    nanl_0100();
    return t_status;
}

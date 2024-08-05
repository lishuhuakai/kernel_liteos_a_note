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

#include <stdlib.h>
#include "functionalext.h"

/**
 * @tc.name      : aligned_alloc_0100
 * @tc.desc      : Verify that memory allocation is successful (parameters are valid)
 * @tc.level     : Level 0
 */
void aligned_alloc_0100(void)
{
    int pagesize = getpagesize();
    void *p = aligned_alloc(pagesize, 1024 * sizeof(int));
    if (!p) {
        t_error("%s aligned_alloc failed\n", __func__);
    }
}

int main(void)
{
    aligned_alloc_0100();
    return t_status;
}
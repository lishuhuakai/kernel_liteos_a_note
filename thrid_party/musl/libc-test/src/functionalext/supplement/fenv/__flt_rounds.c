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

#include <stdio.h>
#include <math.h>
#include <fenv.h>
#include <float.h>
#include "test.h"

/**
 * @tc.name      : sendmmsg_0100
 * @tc.desc      : Test __flt_rounds to get the floating point value mode
 * @tc.level     : Level 0
 */
void __flt_rounds_0100(void)
{
    int result = __flt_rounds();
    if (result != 1) {
        t_error("%s __flt_rounds error get result is %d are not want 1\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    __flt_rounds_0100();
    return t_status;
}
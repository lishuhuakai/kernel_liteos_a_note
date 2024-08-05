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
 * @tc.name      : erand48_0100
 * @tc.desc      : Verify that a positive random floating-point number is generated (valid argument)
 * @tc.level     : Level 0
 */
void erand48_0100(void)
{
    unsigned short xseed[3] = {1, 2, 3};
    double result = erand48(xseed);
    EXPECT_TRUE("erand48_0100", result >= 0.0 && result < 1.0);
}

int main(int argc, char *argv[])
{
    erand48_0100();
    return t_status;
}
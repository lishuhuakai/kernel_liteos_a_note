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

#include <stdlib.h>

#include "functionalext.h"
#include "stdio.h"

const int32_t PUTC_RET = 112;

/**
 * @tc.name      : putchar_0100
 * @tc.desc      : Verify putchar process success
 * @tc.level     : Level 0
 */
void putchar_0100(void)
{
    int32_t ret = putchar('p');
    EXPECT_EQ("putchar_0100", ret, PUTC_RET);
}

int main(int argc, char *argv[])
{
    putchar_0100();
    return t_status;
}

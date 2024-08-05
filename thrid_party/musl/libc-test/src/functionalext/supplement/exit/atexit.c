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

#include <stdio.h>
#include <stdlib.h>
#include "test.h"

void atexit_0100(void)
{
    printf("%s this is end msg\n", __func__);
}

/**
 * @tc.name      : atexit_0100
 * @tc.desc      : Execute function when testing atexit method registration and exit
 * @tc.level     : Level 0
 */
int main(int argc, char *argv[])
{
    int result = atexit(atexit_0100);
    if (result != 0) {
        t_error("atexit_0100 error get result is not 0\n");
    }
    return t_status;
}
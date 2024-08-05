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

#include <unistd.h>
#include <stdio.h>
#include "test.h"

/**
 * @tc.name      : getdtablesize_0100
 * @tc.desc      : Test getdtablesize to get the maximum number of files that can be opened by a process
 * @tc.level     : Level 0
 */
void getdtablesize_0100(void)
{
    int result = getdtablesize();
    if (result == 0) {
        t_error("%s getdtablesize error get result is 0\n", __func__);
    }
}
int main(int argc, char *argv[])
{
    getdtablesize_0100();
    return t_status;
}
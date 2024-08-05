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
#include <fcntl.h>
#include "test.h"

/**
 * @tc.name      : readahead_0100
 * @tc.desc      : Test file readahead for readahead method
 * @tc.level     : Level 2
 */
void readahead_0100(void)
{
    int result = readahead(-1, 0, 123);
    int errcode = -1;
    if (result != errcode) {
        t_error("%s readahead error get result is %d are not want -1\n", __func__, result);
    }
}

int main(int argc, char *argv[])
{
    readahead_0100();
    return t_status;
}
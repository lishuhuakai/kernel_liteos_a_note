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
#include <string.h>
#include <wchar.h>
#include "test.h"

/**
 * @tc.name      : wcslen_0100
 * @tc.desc      : Calculate the length of a wide character array
 * @tc.level     : Level 0
 */
void wcslen_0100(void)
{
    wchar_t str[] = L"wcslen";
    int target = 6;
    size_t num = wcslen(str);
    if (num != target)
        t_error("%s wcslen is not right", __func__);
}

int main(int argc, char *argv[])
{
    wcslen_0100();
    return t_status;
}
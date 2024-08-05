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
 * @tc.name      : wcpcpy_0100
 * @tc.desc      : Test the wcpcpy method to copy a wide string in bits
 * @tc.level     : Level 0
 */
void wcpcpy_0100(void)
{
    wchar_t src[] = L"Source string";
    wchar_t dst[14];
    wchar_t *result = wcpcpy(dst, src);
    if (wcscmp(result, dst + 13) != 0) {
        t_error("%s wcpcpy get result is %ls are not want %ls\n", __func__, result, dst + 13);
    }
    if (wcscmp(src, dst) != 0) {
        t_error("%s wcpcpy get dst is %ls are not want %ls\n", __func__, dst, src);
    }
}

int main(int argc, char *argv[])
{
    wcpcpy_0100();
    return t_status;
}
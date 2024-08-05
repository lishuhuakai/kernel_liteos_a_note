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

#include <wchar.h>
#include "functionalext.h"

/**
 * @tc.name      : getwc_0100
 * @tc.desc      : Verify that wide characters can be obtained from the file (parameters are valid)
 * @tc.level     : Level 0
 */
void getwc_0100(void)
{
    const char *ptr = "/data//test.txt";
    FILE *fptr = fopen(ptr, "w+");
    EXPECT_PTRNE("getwc_0100", fptr, NULL);
    wchar_t wc = L'p';
    char ret = fputwc(wc, fptr);
    rewind(fptr);
    wint_t result = getwc(fptr);
    EXPECT_EQ("getwc_0100", (wchar_t)result, wc);
    fclose(fptr);
    remove(ptr);
}

int main(void)
{
    getwc_0100();
    return t_status;
}

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

#include <stdio_ext.h>
#include "functionalext.h"

/**
 * @tc.name      : __fseterr_0100
 * @tc.desc      : Set stream in error
 * @tc.level     : Level 0
 */
void __fseterr_0100(void)
{
    FILE *fp = fopen("/dev/null", "w");

    EXPECT_FALSE("__fseterr_0100", ferror(fp));
    __fseterr(fp);
    EXPECT_TRUE("__fseterr_0100", ferror(fp));
    clearerr(fp);
    EXPECT_FALSE("__fseterr_0100", ferror(fp));
    fclose(fp);
}

int main(int argc, char *argv[])
{
    __fseterr_0100();
    return t_status;
}
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

#include <dirent.h>
#include "functionalext.h"

/**
 * @tc.name      : alphasort_0100
 * @tc.desc      : Verify that the directory structure can be sorted alphabetically
 * @tc.level     : Level 0
 */
void alphasort_0100(void)
{
    struct dirent **namelist;
    int total = scandir("/", &namelist, 0, alphasort);
    EXPECT_TRUE("alphasort_0100", total > 0);
}

/**
 * @tc.name      : alphasort_0200
 * @tc.desc      : Verify that the directory structure cannot be sorted alphabetically
 * @tc.level     : Level 2
 */
void alphasort_0200(void)
{
    struct dirent **namelist;
    int total = scandir(NULL, &namelist, 0, alphasort);
    EXPECT_TRUE("alphasort_0100", total < 0);
}

int main(int argc, char *argv[])
{
    alphasort_0100();
    alphasort_0200();
    return t_status;
}
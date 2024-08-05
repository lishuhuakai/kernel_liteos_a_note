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

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "functionalext.h"

#define LENGTH 50

/**
 * @tc.name      : strcat_0100
 * @tc.desc      : Verify strcat process success
 * @tc.level     : Level 0
 */
void strcat_0100(void)
{
    char src[LENGTH] = "Source";
    char *ret = strcat(src, "Destination");
    EXPECT_STREQ("strcat_0100", ret, "SourceDestination");
}

int main(void)
{
    strcat_0100();
    return t_status;
}

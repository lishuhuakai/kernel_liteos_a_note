/**
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

#include "functionalext.h"

/**
 * @tc.name      : remove_0100
 * @tc.desc      : The parameter is valid and the file can be deleted.
 * @tc.level     : Level 0
 */
void remove_0100(void)
{
    const char *ptr = "test.txt";
    FILE *fptr = fopen(ptr, "w");
    EXPECT_TRUE("remove_0100", fptr != NULL);
    fclose(fptr);
    int ret = remove(ptr);
    EXPECT_EQ("remove_0100", ret, 0);
    fptr = NULL;
    ptr = NULL;
}

/**
 * @tc.name      : remove_0200
 * @tc.desc      : The parameter is invalid and the file cannot be deleted.
 * @tc.level     : Level 2
 */
void remove_0200(void)
{
    const char *ptr = "test.txt";
    int ret = remove(ptr);
    EXPECT_EQ("remove_0200", ret, -1);
    ptr = NULL;
}

int main(int argc, char *argv[])
{
    remove_0100();
    remove_0200();
    return t_status;
}
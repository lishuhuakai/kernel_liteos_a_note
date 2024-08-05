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

#include <sys/ipc.h>
#include "functionalext.h"
#include "filepath_util.h"

/*
 * @tc.name      : ftok_0100
 * @tc.desc      : Verify that the ket_t value can be obtained (the parameter is valid)
 * @tc.level     : Level 0
 */
void ftok_0100(void)
{
    char path[PATH_MAX] = {0};
    FILE_ABSOLUTE_PATH("ftok", path);
    key_t result = ftok(path, 1);
    EXPECT_NE("ftok_0100", result, -1);
}

/*
 * @tc.name      : ftok_0200
 * @tc.desc      : Validation cannot get ket_t value (parameter invalid)
 * @tc.level     : Level 2
 */
void ftok_0200(void)
{
    key_t result = ftok("./test", 1);
    EXPECT_EQ("ftok_0200", result, -1);
}
int main(int argc, char *argv[])
{
    ftok_0100();
    ftok_0200();

    return t_status;
}
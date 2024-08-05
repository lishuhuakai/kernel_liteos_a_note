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

#include <mntent.h>
#include "functionalext.h"

/**
 * @tc.name      : endmntent_0100
 * @tc.desc      : Verify that the stream and its associated file system description file can be closed
 * @tc.level     : Level 0
 */
void endmntent_0100(void)
{
    FILE *fp = fopen("/data/test.txt", "w+");
    EXPECT_PTRNE("endmntent_0100", fp, NULL);

    int result = endmntent(fp);
    EXPECT_EQ("endmntent_0100", result, 1);
    remove("/data/test.txt");
}

int main(int argc, char *argv[])
{
    endmntent_0100();
    return t_status;
}
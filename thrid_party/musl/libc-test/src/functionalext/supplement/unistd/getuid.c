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

#include <stdlib.h>
#include <unistd.h>
#include "functionalext.h"

/**
 * @tc.name      : getuid_0100
 * @tc.desc      : Can get the UID of the current process.
 * @tc.level     : Level 0
 */
void getuid_0100(void)
{
    char struid[10];
    system("id -u > ./id.txt");
    FILE *fptr = fopen("id.txt", "r");
    EXPECT_PTRNE("getuid_0100", fptr, NULL);
    fread(struid, sizeof(struid), 1, fptr);
    int intuid = atoi(struid);
    uid_t uid = getuid();
    remove("id.txt");
    EXPECT_EQ("getuid_0100", uid, intuid);
}

int main(int argc, char *argv[])
{
    getuid_0100();
    return t_status;
}
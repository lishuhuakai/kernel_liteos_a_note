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

#include "functionalext.h"

/**
 * @tc.name      : execlp_0100
 * @tc.desc      : Each parameter is valid, and the specified file can be executed.
 * @tc.level     : Level 0
 */
void execlp_0100(void)
{
    pid_t fpid;
    fpid = fork();
    if (fpid == 0) {
        int ret = execlp("touch", "touch", "execlptest.txt", NULL);
        EXPECT_NE("execlp_0100", ret, -1);
    }
    sleep(1);

    int isExist = access("execlptest.txt", F_OK);
    EXPECT_EQ("execlp_0100", isExist, 0);
    remove("execlptest.txt");
}

/**
 * @tc.name      : execlp_0200
 * @tc.desc      : The content pointed to by the file parameter is empty, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execlp_0200(void)
{
    int ret = execlp(" ", "touch", "execlptest.txt", NULL);
    EXPECT_EQ("execlp_0200", ret, -1);
}

/**
 * @tc.name      : execlp_0300
 * @tc.desc      : The length of file exceeds NAME_MAX, and the specified file cannot be executed.
 * @tc.level     : Level 2
 */
void execlp_0300(void)
{
    char buff[300];
    for (int i = 0; i < 300; i++) {
        buff[i] = 'a';
    }
    int ret = execlp(buff, "touch", "execlptest.txt", NULL);
    EXPECT_EQ("execlp_0300", ret, -1);
}

int main(int argc, char *argv[])
{
    execlp_0100();
    execlp_0200();
    execlp_0300();
    return t_status;
}